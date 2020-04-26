#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include <filesystem>
#include <map>

using namespace std;

auto fCapture = [=](string str) { return "(" + str + ")";};
auto fExpr = [=](string str) { return "(?:" + str + ")";};
auto fOptional = [=](string str) { return fExpr(fExpr(str) + "?"); };
auto fOr = [=](string x, string y) { return fExpr(x + "|" + y); };
auto fAdd = [=](string str) { return fExpr(str) + "+"; };
auto fStar = [=](string str) { return fExpr(str) + "*"; };
auto fParenthesis = [=](string str) { return "\\(" + str + "\\)"; };
auto fStr = [=](string str) { return "\"" + str + "\""; };
auto fCaptureStr = [=]() { return fStr(fCapture("[^\"]*")); };

const string sID = "[_a-zA-Z][_a-zA-Z0-9]*";
const string sClass = fOr("struct", "class");
const string sNS = fStar(sID + "::") + sID;
const string sMeta = sID + fOptional(fParenthesis(fStr("[^\"]*")));
const string sMetas = fOptional(fStar(sMeta + ",") + sMeta);
const string sAttrs = "\\[\\[" + fCapture(sMetas) + "\\]\\]";

struct VarMeta {
    map<string, string> attrs;
    string prefix;
    string type;
    string name;

    void Print(const std::string& prefix) const {
        cout << prefix << "[VarMeta]" << endl
            << prefix << "- name: " << name << endl
            << prefix << "- type: " << type << endl
            << prefix << "- prefix: " << this->prefix << endl
            << prefix << "- attrs " << endl;
        for (const auto& [k, v] : attrs) {
            cout << "  - " << k << ": " << v << endl;
        }
    }
};

struct ClassMeta {
    string ns; // namespace
    string type; // class, struct
    map<string, string> attrs;
    string name;
    vector<VarMeta> varmetas;

    void Print() const {
        cout << "[ClassMeta]" << endl
            << "- name: " << name << endl
            << "- type: " << type << endl
            << "- namespace: " << ns << endl
            << "- attrs: " << endl
            << "- varmetas: " << endl;
        for (const auto& [k, v] : attrs)
            cout << "  - " << k << ": " << v << endl;
        for (const auto& varmeta : varmetas)
            varmeta.Print("  ");
    }
};

string ReadFile(const string& path) {
    std::ifstream t(path);
    if (!t.is_open())
        return "";
    std::stringstream buffer;
    buffer << t.rdbuf();

    auto data = buffer.str();

    /*cout << "[file]" << endl
        << data << endl
        << "--------------" << endl;*/

    return data;
}

string SimplifyFile(string data) {
    string sLineComment = "//.*\\n";
    string sMultilineComment = "/\\*" + fStar("[^\\*]*\\*[^/]") + "[^\\*]*\\*/";
    string sComment = fOr(sLineComment, sMultilineComment);

    data = regex_replace(data, regex{ sComment }, "");
    data = regex_replace(data, regex{ "[^\"^\n]*\\#.*\n" }, "\n"); // #define, ...
    data = regex_replace(data, regex{ "noexcept|inline" }, ""); // useless keywords
    data = regex_replace(data, regex{ "protected:" }, "private:"); // useless keywords
    data = regex_replace(data, regex{ "[ \t\n]+" }, " "); // zip spaces
    data = regex_replace(data, regex{ "\\[\\[(?: ?)\\]\\]" }, ""); // del empty attrs
    data = regex_replace(data, regex{ "(?: ?)(,|=|;|:|::|\\*|\"|\\(|\\)|<|>|\\[|\\]|\\{|\\})(?: ?)" }, "$1"); // useless spaces
    data = regex_replace(data, regex{ "\\]\\]\\[\\[" }, ","); // attr cat
    data = regex_replace(data, regex{ "=\"\"" }, ""); // =""
    data = regex_replace(data, regex{ "\"\"" }, ""); // str cat
    data = regex_replace(data, regex{ "alignas\\(.*?\\)" }, ""); // alignas
    data = regex_replace(data, regex{ "(using|typedef|friend) [^;]*;" }, ""); // using, typedef
    data = regex_replace(data, regex{ "(struct|class) [^;^\\{]*;" }, ""); // forward declaration
    data = regex_replace(data, regex{ "namespace " + sNS + "\\{\\}" }, ""); // empty namespace

    string match_template = "(template<[^>]*)<.*?>";
    while (regex_search(data, regex{ match_template }))
        data = regex_replace(data, regex{ match_template }, "$1");
    data = regex_replace(data, regex{ "template<.*?>" }, ""); // template

    cout << "[step 1] simplify file" << endl
        << data << endl
        << "--------------" << endl;

    return data;
}

string SimplifyBody(string body) {
    string mathc_lr = "(\\{[^\\}]*)\\{.*?\\}(?:;?)";
    while (regex_search(body, regex{ mathc_lr }))
        body = regex_replace(body, regex{ mathc_lr }, "$1;");
    body = regex_replace(body, regex{ "\\{.*?\\}(?:;?)" }, ";");
    body = regex_replace(body, regex{ ";(;|,)" }, "$1");
    body = regex_replace(body, regex{ ":" + fStar(sID + ",") + sID + ";" }, ";");
    body = regex_replace(body, regex{ "=.*?(;|,|\\))" }, "$1");
    body = regex_replace(body, regex{ sClass + ".*?;" }, "");
    /*cout << "[step 3] delete: {.*}, constructor init list, =.*?(;|,|)), inner struct/class" << endl
        << "simplified body: " << body << endl << endl;*/

    return body;
}

string GetPublic(string classType, string body) {
    cout << body << endl;
    smatch match_public;
    if (classType == "struct")
        body = "public:" + body;
    body += "private:";
    string in_public;
    auto biter = body.cbegin();
    auto eiter = body.cend();
    while(regex_search(biter, eiter, match_public, regex{ "public:" + fCapture(".*?") + "private:" })) {
        in_public = match_public[1];
        biter = match_public[0].second;
    }
    cout << "[step 4] find public" << endl
        << "- in_public: " << in_public << endl << endl;
    return in_public;
}

map<string, string> GenAttrs(const string& data) {
    map<string, string> attrs;
    auto biter = data.cbegin();
    auto eiter = data.cend();
    smatch match;
    while (regex_search(biter, eiter, match, regex{ fCapture(sID) + fOptional(fParenthesis(fCaptureStr())) })) {
        VarMeta varmeta;
        attrs[match[1]] = match[2];
        biter = match[0].second;
    }
    return attrs;
};

vector<VarMeta> GenVarMeta(string in_public) {
    // delete <...>
    string match_template = "(<[^>]*)<.*?>";
    while (regex_search(in_public, regex{ match_template }))
        in_public = regex_replace(in_public, regex{ match_template }, "$1");
    in_public = regex_replace(in_public, regex{ "<.*?>" }, " "); // template
    cout << in_public << endl;

    vector<VarMeta> rst;
    string sPrefix = fStar(fOr("static", "constexpr") + " ");
    string sVar = fOptional("\\[\\[" + fCapture(sMetas) + "\\]\\]")
        + fCapture("(?:(?:(?:static|constexpr) )*)") // prefix
        + fCapture(fOptional("const ") + fStar(sID + "::") + sID + fExpr(" |\\[\\]|\\*|&") + fOptional("const")) // type prefix
        + fCapture(sID)
        + fCapture(fOptional("\\[" + fOptional("[1-9][0-9]*") + "\\]")) // type postfix
        + ";";
    auto biter_in_public = in_public.cbegin();
    auto eiter_in_public = in_public.cend();
    smatch match_in_public;
    /*cout << "[step 5] vars" << endl;*/
    string keys_vars[] = { "var", "attrs", "prefix", "type_prefix", "var_name", "type_postfix" };
    while (regex_search(biter_in_public, eiter_in_public, match_in_public, regex{ sVar })) {
        VarMeta varmeta;

        varmeta.name = match_in_public[4];
        string typeprefix = match_in_public[3].str();
        if (typeprefix.back() == ' ')
            typeprefix.pop_back();
        varmeta.type = typeprefix + match_in_public[5].str();
        varmeta.prefix = match_in_public[2];
        varmeta.attrs = GenAttrs(match_in_public[1]);

        rst.push_back(varmeta);

        biter_in_public = match_in_public[0].second;
    }

    return rst;
}

// begin -> l, end-> r
tuple<string::const_iterator, string::const_iterator> FindRange(char l, char r, string::const_iterator b) {
    while (*b != l)
        b++;

    size_t n = 1;
    auto cur = b;
    while (n > 0) {
        cur++;
        if (*cur == l)
            n++;
        else if (*cur == r)
            n--;
    }

    return { b , cur };
}

ClassMeta GenClassMeta(string ns, string data) {
    // [step 2] class, class attrs, class_name, body
    ClassMeta classMeta;

    string body;

    string sAttrs = "\\[\\[" + fCapture(sMetas) + "\\]\\]";
    smatch match_all;
    string pattern_all = fCapture(sClass)
        + fOr(sAttrs, " ")
        + fCapture(sID)
        + fOptional(":[^\\{]*")
        //+ range('{', '}', true)
        ;
    /*cout << "[step 2] class, class attrs, class_name, body" << endl << endl;*/
    regex_search(data, match_all, regex{ pattern_all });
    string keys_all[] = { "all", "class", "attr", "class_name" };
    /*for (size_t i = 0; i < match_all.size(); i++) {
        cout << "- " << keys_all[i] << ": " << match_all[i] << endl;
    }*/

    string tmp(match_all[0].second, data.cend());
    auto [toBegin, toEnd] = FindRange('{', '}', match_all[0].second);
    body = string(toBegin + 1, toEnd);
    //cout << "- body: " << body << endl << endl;

    classMeta.attrs = GenAttrs(match_all[2]);
    classMeta.name = match_all[3];
    classMeta.type = match_all[1];
    classMeta.ns = ns;

    // [step 3] delete: {.*}, constructor init list, =.*?;, inner struct/class
    body = SimplifyBody(body);

    // [step 4] find public
    auto in_public = GetPublic(classMeta.type, body);

    // [step 5] vars
    classMeta.varmetas = GenVarMeta(in_public);

    return classMeta;
}

vector<ClassMeta> ParseClassMetas(string ns, string data) {
    vector<ClassMeta> rst;

    auto biter = data.cbegin();
    auto eiter = data.cend();
    smatch match;
    while (regex_search(biter, eiter, match, regex{ sClass + ".*?\\{" })) {
        auto [toL, toR] = FindRange('{', '}', match[0].first);
        string classData(match[0].first, toR + 1);
        rst.push_back(GenClassMeta(ns, classData));
        biter = toR + 1;
    }

    return rst;
}

vector<ClassMeta> ParseNameSpaceLevel(const string& preNS, string data) {
    vector<ClassMeta> rst;

    auto biter = data.cbegin();
    auto eiter = data.cend();
    smatch match;
    while (regex_search(biter, eiter, match, regex{ "namespace " + fCapture(sNS) })) {
        string ns = match[1];
        if (!preNS.empty())
            ns = preNS + "::" + ns;
        auto [toBegin, toEnd] = FindRange('{', '}', match[0].second);
        string nsBody(toBegin + 1, toEnd);
        auto classMetas = ParseNameSpaceLevel(ns, nsBody);
        for (const auto& meta : classMetas)
            rst.push_back(meta);

        biter = toEnd + 1;
    }

    string tailData(biter, data.cend());
    auto tailMetas = ParseClassMetas(preNS, tailData);
    for (const auto& meta : tailMetas)
        rst.push_back(meta);

    return rst;
}

vector<ClassMeta> ParseFile(const string& path) {
    auto data = ReadFile(path);

    data = SimplifyFile(data);

    return ParseNameSpaceLevel("", data);
}

void WriteFile(const string& path, const std::vector<ClassMeta>& classMetas) {
    ofstream file(path);
    cout << "write " << path << endl;

    string null = "null";

    file << "/*" << endl
        << "    Generated by Ubpa::UScene_parser"<< endl
        << "    Any changes to this file will be overwritten by the next CMake run" << endl
        << "    ----" << endl
        << "    Usage:" << endl
        << "        #include \"this-file\" // in cpp" << endl
        << "*/" << endl << endl;

    file << "#include <UDP/Reflection/Reflection.h>" << endl << endl;
    file << "#include <UScene/core/SObj.h>" << endl << endl;

    for (const auto& classMeta : classMetas) {
        file << "namespace " << classMeta.ns << "::detail::dynamic_reflection {" << endl;
        file << "    void ReflRegist_" << classMeta.name << "() {" << endl;
        file << "        Reflection<" << classMeta.name << ">::Instance() // name : "
            << classMeta.type << " " << classMeta.ns << "::" << classMeta.name << endl;
        for (const auto& [k,v] : classMeta.attrs)
            file << "            .Regist(\"" << k << "\", \"" << v << "\")"<< endl;
        for (const auto& varmeta : classMeta.varmetas) {
            if (varmeta.prefix.find("static") != string::npos
                || varmeta.prefix.find("constexpr") != string::npos)
            {
                file << "            //.Regist(&" << classMeta.name << "::" << varmeta.name << ", \"" << varmeta.name << "\")"
                    " // " << varmeta.prefix << " " << varmeta.type << endl;
            }
            else {
                file << "            .Regist(&" << classMeta.name << "::" << varmeta.name << ", \"" << varmeta.name << "\")"
                    " // " << varmeta.prefix << " " << varmeta.type << endl;
            }
            for (const auto& [k, v] : varmeta.attrs)
                file << "            .Regist(\"" << varmeta.name << "\", \"" << k << "\", \"" << (v.empty() ? null : v) << "\")" << endl;
        }
        file << "            ;" << endl;
        file << "        if constexpr (std::is_base_of_v<Component, " << classMeta.name <<">) {" << endl;
        file << "            Reflection<" << classMeta.name << ">::Instance().RegistConstructor([](SObj* sobj) {" << endl;
        file << "                if constexpr (std::is_base_of_v<Component, " << classMeta.name << ">) {" << endl;
        file << "                    if constexpr (Ubpa::detail::SObj_::IsNecessaryCmpt<" << classMeta.name << ">)" << endl;
        file << "                        return sobj->Get<" << classMeta.name << ">();" << endl;
        file << "                    else" << endl;
        file << "                        return sobj->GetOrAttach<" << classMeta.name << ">();" << endl;
        file << "                };" << endl;
        file << "            });" << endl;
        file << "        }" << endl;
        file << "    }" << endl;
        file << "}" << endl;
    }
}

int main(int argn, char** argc)
{
    string dir = argc[1];
    if (!filesystem::is_directory(dir))
        filesystem::create_directories(dir);
    for (int i = 2; i < argn; i++) {
        string path = argc[i];
        size_t pos_backslash = path.find_last_of('/');
        size_t pos_dot = path.find_last_of('.');
        string filename = path.substr(pos_backslash + 1, pos_dot - pos_backslash);
        string outpath = dir + "/" + filename + "inl";
        auto rst = ParseFile(path);
        for (const auto& meta : rst)
            meta.Print();
        WriteFile(outpath, rst);
    }

    return 0;
}
