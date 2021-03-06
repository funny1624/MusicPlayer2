#pragma once
#include "stdafx.h"

class IPlayerUI
{
public:
	struct UIButton		//界面中绘制的按钮
	{
		CRect rect;				//按钮的矩形区域
		bool hover{ false };	//鼠标是否指向按钮
		bool last_hover{ false };
		bool enable{ true };	//按钮是否启用
	};

public:
	IPlayerUI(){}
	~IPlayerUI(){}

	virtual void Init(CDC* pDC) = 0;
	virtual void DrawInfo(bool reset = false) = 0;

	virtual void RButtonUp(CPoint point) = 0;
	virtual void MouseMove(CPoint point) = 0;
	virtual void LButtonUp(CPoint point) = 0;
	virtual void OnSizeRedraw(int cx, int cy) = 0;

	virtual CRect GetThumbnailClipArea() = 0;

	virtual void UpdateToolTipPosition() = 0;
};