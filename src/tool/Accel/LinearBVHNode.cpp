#include <UScene/tool/Accel/LinearBVHNode.h>

using namespace Ubpa;
using namespace std;

void LinearBVHNode::InitLeaf(const bboxf3& box, int shapesOffset, int shapesNum) {
	this->box = box;
	this->shapesOffset = shapesOffset;
	this->shapesNum = shapesNum;
	axis = Axis::INVALID;
}

void LinearBVHNode::InitBranch(const bboxf3& box, int secondChildIdx, Axis axis) {
	this->box = box;
	this->secondChildIdx = secondChildIdx;
	shapesNum = 0;
	this->axis = axis;
}

const vector<size_t> LinearBVHNode::PrimitiveIndices() const {
	assert(IsLeaf());
	vector<size_t> rst;
	for (size_t i = 0; i < static_cast<size_t>(shapesNum); i++)
		rst.push_back(shapesOffset + i);
	return rst;
}