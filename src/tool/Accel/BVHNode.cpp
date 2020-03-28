#include "BVHNode.h"

#include <UScene/core/Primitive/Primitive.h>

using namespace Ubpa;
using namespace std;

BVHNode::BVHNode(const unordered_map<const Primitive*, bboxf3>& p2b,
	vector<const Primitive*>& primitives,
	size_t primitiveOffset, size_t primitiveNum)
	: primitiveOffset(primitiveOffset), primitiveNum(primitiveNum)
{
	Build(p2b, primitives);
}

BVHNode::~BVHNode() {
	delete l;
	delete r;
}

void BVHNode::Build(const unordered_map<const Primitive*, bboxf3>& p2b, vector<const Primitive*>& primitives) {
	// Build bvh form primitiveOffset to primitiveOffset + primitiveNum

	constexpr size_t bucketNum = 12;
	constexpr size_t maxLeafSize = 4;
	constexpr double t_trav = 0.125;

	bboxf3 extentBox;
	for (size_t i = primitiveOffset; i < primitiveOffset + primitiveNum; i++) {
		auto target = p2b.find(primitives[i]);
		assert(target != p2b.cend());
		const auto worldBox = target->second;

		extentBox.combine_to_self(worldBox.center());
		box.combine_to_self(worldBox);
	}

	if (primitiveNum <= maxLeafSize) {
		l = nullptr;
		r = nullptr;
		return;
	}

	// get best partition
	vector<const Primitive*> bestPartition[2];
	double minCost = DBL_MAX;
	for (int dim = 0; dim < 3; dim++) {

		// 1. compute buckets

		vector<vector<const Primitive*>> buckets(bucketNum);
		vector<bboxf3> boxesOfBuckets(bucketNum);
		{// 将 shape 放到 bucket 中，计算好每个 bucket 的 box
			double bucketLen = extentBox.diagonal()[dim] / bucketNum;
			if (bucketLen == 0)
				continue;

			double left = extentBox.minP()[dim];
			for (size_t i = primitiveOffset; i < primitiveOffset + primitiveNum; i++) {
				auto target = p2b.find(primitives[i]);
				assert(target != p2b.cend());
				const auto& worldBox = target->second;

				double center = worldBox.center()[dim];
				size_t bucketID = std::clamp(static_cast<size_t>((center - left) / bucketLen),
					static_cast<size_t>(0),
					bucketNum - 1);

				buckets[bucketID].push_back(primitives[i]);
				boxesOfBuckets[bucketID].combine_to_self(worldBox);
			}
		}

		// 2. accumulate buckets

		vector<bboxf3> leftBox(bucketNum);
		vector<size_t> leftAccNum(bucketNum);
		vector<bboxf3> rightBox(bucketNum);
		vector<size_t> rightAccNum(bucketNum);
		leftAccNum[0] = 0;
		rightAccNum[0] = 0;
		for (size_t i = 1; i <= bucketNum - 1; i++) {
			leftBox[i] = leftBox[i - 1].combine(boxesOfBuckets[i - 1]);
			leftAccNum[i] = leftAccNum[i - 1] + buckets[i - 1].size();

			rightBox[i] = rightBox[i - 1].combine(boxesOfBuckets[bucketNum - i]);
			rightAccNum[i] = rightAccNum[i - 1] + buckets[bucketNum - i].size();
		}

		// 3. get best partition of dim
		size_t bestLeftBucketsNum = 0;
		double minCostOfDim = DBL_MAX;
		for (int leftNum = 1; leftNum <= bucketNum - 1; leftNum++) {
			int rightNum = bucketNum - leftNum;

			double leftS = leftBox[leftNum].area();
			double rightS = rightBox[rightNum].area();

			double curCost = t_trav + leftS * leftAccNum[leftNum] + rightS * rightAccNum[rightNum];

			if (curCost < minCostOfDim) {
				bestLeftBucketsNum = leftNum;
				minCostOfDim = curCost;
			}
		}

		// 4. set best partition
		if (minCostOfDim < minCost) {
			bestPartition[0].clear();
			bestPartition[1].clear();

			minCost = minCostOfDim;
			axis = static_cast<Axis>(dim);
			for (size_t i = 0; i < bestLeftBucketsNum; i++) {
				for (auto primitive : buckets[i])
					bestPartition[0].push_back(primitive);
			}
			for (size_t i = bestLeftBucketsNum; i < bucketNum; i++) {
				for (auto primitive : buckets[i])
					bestPartition[1].push_back(primitive);
			}
		}
	}

	// recursion
	if (bestPartition[0].size() == 0 || bestPartition[1].size() == 0)
		return;

	for (size_t i = 0; i < bestPartition[0].size(); i++)
		primitives[i + primitiveOffset] = bestPartition[0][i];
	for (size_t i = 0; i < bestPartition[1].size(); i++)
		primitives[i + primitiveOffset + bestPartition[0].size()] = bestPartition[1][i];

	l = new BVHNode(p2b, primitives, primitiveOffset, bestPartition[0].size());
	r = new BVHNode(p2b, primitives, primitiveOffset + bestPartition[0].size(), bestPartition[1].size());
}
