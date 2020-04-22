#pragma once
#include "engine.h"

class Enemies{
public:
	static std::shared_ptr<Entity> makeSkeleton(Scene* scene);
	static std::shared_ptr<Entity> makeSkeletonArcher(Scene* scene);
	static std::shared_ptr<Entity> makeSkeletonChief(Scene* scene);
	static std::shared_ptr<Entity> makeBoss(Scene* scene);
	Enemies() = delete;
};