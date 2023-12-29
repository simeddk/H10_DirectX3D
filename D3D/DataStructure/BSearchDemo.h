#pragma once

#include "Systems/IExecute.h"

struct DataDesc
{
	UINT Index;
	UINT Data;
};

class BSearchDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override {};
	virtual void Update() override {};
	virtual void PreRender() override {};
	virtual void Render() override {};
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void ReadFile(string path);

	UINT SequenceSearch(UINT target, int& count);
	UINT BinarySearch(UINT target, int& count);

	//int Compare(const void* data1, const void* data2);
	
private:
	DataDesc datas[USHRT_MAX];
	int count = 0;
};