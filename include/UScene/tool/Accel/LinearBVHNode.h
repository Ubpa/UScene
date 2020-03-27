#pragma once

#include <UGM/bbox.h>

namespace Ubpa {
	class LinearBVHNode {
	public:
		void InitLeaf(const bboxf3& box, int shapesOffset, int shapesNum) {
			this->box = box;
			this->shapesOffset = shapesOffset;
			this->shapesNum = shapesNum;
			axis = Axis::INVALID;
		}

		void InitBranch(const bboxf3& box, int secondChildIdx, Axis axis) {
			this->box = box;
			this->secondChildIdx = secondChildIdx;
			shapesNum = 0;
			this->axis = axis;
		}

	public:
		const bboxf3& GetBox() const { return box; }
		bool IsLeaf() const { return shapesNum != 0; }
		const std::vector<size_t> ShapesIdx() const {
			assert(IsLeaf());
			std::vector<size_t> rst;
			for (uint16_t i = 0; i < shapesNum; i++)
				rst.push_back(shapesOffset + i);
			return rst;
		}
		static size_t FirstChildIdx(size_t nodeIdx) { return nodeIdx + 1; }
		size_t GetSecondChildIdx() const {
			assert(!IsLeaf());
			return secondChildIdx;
		}
		Axis GetAxis() const {
			assert(!IsLeaf());
			return axis;
		}

	private:
		bboxf3 box;
		union {
			uint32_t shapesOffset; // leaf
			uint32_t secondChildIdx; // interior
		};
		uint16_t shapesNum;
		Axis axis;
		const uint8_t pad[1]{ 0 }; // ensure 32 byte total size
	};
	static_assert(sizeof(LinearBVHNode) == 32);
}
