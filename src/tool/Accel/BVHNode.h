#pragma once

#include <UGM/bbox.h>

#include <vector>
#include <unordered_map>

namespace Ubpa {
	class Primitive;

	class BVHNode {
	public:
		BVHNode(const std::unordered_map<Primitive*, bboxf3>& p2b,
			std::vector<Primitive*>& primitives,
			size_t primitiveOffset, size_t primitiveNum);

		~BVHNode();

		bool IsLeaf() const { return l == nullptr && r == nullptr; }
		const bboxf3& GetBBox() const { return box; }
		size_t GetPrimitiveOffset() const { return primitiveOffset; }
		size_t GetPrimitiveNum() const { return primitiveNum; }
		Axis GetAxis() const { return axis; }
		BVHNode* GetL() const { return l; }
		BVHNode* GetR() const { return r; }

	private:
		// 调整 primitives 的顺序，设置 l, r, box 和 axis
		void Build(const std::unordered_map<Primitive*, bboxf3>& p2b, std::vector<Primitive*>& primitives);

	private:
		bboxf3 box;
		size_t primitiveOffset;
		size_t primitiveNum;
		Axis axis{ Axis::INVALID };
		BVHNode* l{ nullptr };
		BVHNode* r{ nullptr };
	};
}
