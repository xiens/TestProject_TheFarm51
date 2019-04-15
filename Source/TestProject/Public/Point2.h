// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TESTPROJECT_API Point2
{
public:
	Point2();
	Point2(float _x, float _y) :x(_x), y(_y) {};
	~Point2();
	float x;
	float y;
};
