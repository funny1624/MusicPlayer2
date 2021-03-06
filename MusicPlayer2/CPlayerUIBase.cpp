#include "stdafx.h"
#include "CPlayerUIBase.h"


CPlayerUIBase::CPlayerUIBase(UIData& ui_data)
	:m_ui_data(ui_data)
{
	m_popup_menu.LoadMenu(IDR_LYRIC_POPUP_MENU);	//装载歌词右键菜单
	m_main_popup_menu.LoadMenu(IDR_MAIN_POPUP_MENU);
}


CPlayerUIBase::~CPlayerUIBase()
{
}

void CPlayerUIBase::SetToolTip(CToolTipCtrl * pToolTip)
{
	m_tool_tip = pToolTip;
}

void CPlayerUIBase::Init(CDC * pDC)
{
	m_pDC = pDC;
	m_draw.Create(m_pDC, theApp.m_pMainWnd);
	m_pLayout = std::make_shared<SLayoutData>();
}

void CPlayerUIBase::DrawInfo(bool reset)
{
	if (m_first_draw)
	{
		AddToolTips();
	}
	m_first_draw = false;
}

void CPlayerUIBase::RButtonUp(CPoint point)
{
	if (m_buttons[BTN_VOLUME].rect.PtInRect(point) == FALSE)
		m_show_volume_adj = false;

	CPoint point1;		//定义一个用于确定光标位置的位置  
	GetCursorPos(&point1);	//获取当前光标的位置，以便使得菜单可以跟随光标，该位置以屏幕左上角点为原点，point则以客户区左上角为原点
	if (m_buttons[BTN_REPETEMODE].rect.PtInRect(point))		//如果在“循环模式”的矩形区域内点击鼠标右键，则弹出“循环模式”的子菜单
	{
		CMenu* pMenu = m_main_popup_menu.GetSubMenu(0)->GetSubMenu(1);
		if (pMenu != NULL)
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}

}

void CPlayerUIBase::MouseMove(CPoint point)
{
	for (auto& btn : m_buttons)
	{
		btn.second.hover = (btn.second.rect.PtInRect(point) != FALSE);
	}
}

void CPlayerUIBase::LButtonUp(CPoint point)
{
	if (m_buttons[BTN_REPETEMODE].rect.PtInRect(point))	//点击了“循环模式”时，设置循环模式
	{
		theApp.m_player.SetRepeatMode();
		UpdateRepeatModeToolTip();
	}

	if (!m_show_volume_adj)		//如果设有显示音量调整按钮，则点击音量区域就显示音量调整按钮
		m_show_volume_adj = (m_buttons[BTN_VOLUME].rect.PtInRect(point) != FALSE);
	else		//如果已经显示了音量调整按钮，则点击音量调整时保持音量调整按钮的显示
		m_show_volume_adj = (m_volume_up_rect.PtInRect(point) || m_volume_down_rect.PtInRect(point));

	if (m_show_volume_adj && m_volume_up_rect.PtInRect(point))	//点击音量调整按钮中的音量加时音量增加
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	}
	if (m_show_volume_adj && m_volume_down_rect.PtInRect(point))	//点击音量调整按钮中的音量减时音量减小
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	}

	if (m_buttons[BTN_SKIN].rect.PtInRect(point))
	{
		m_buttons[BTN_SKIN].hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_SWITCH_UI);
	}

	if (m_buttons[BTN_EQ].rect.PtInRect(point))
	{
		m_buttons[BTN_EQ].hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_EQUALIZER);
	}

	if (m_buttons[BTN_SETTING].rect.PtInRect(point))
	{
		m_buttons[BTN_SETTING].hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_OPTION_SETTINGS);
	}

	if (m_buttons[BTN_MINI].rect.PtInRect(point))
	{
		m_buttons[BTN_MINI].hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_MINI_MODE);
	}

}

void CPlayerUIBase::OnSizeRedraw(int cx, int cy)
{
	CRect redraw_rect;
	if (!m_ui_data.m_narrow_mode)	//在普通界面模式下
	{
		if (cx < m_ui_data.client_width)	//如果界面宽度变窄了
		{
			//重新将绘图区域右侧区域的矩形区域填充为对话框背景色
			redraw_rect = m_draw_rect;
			redraw_rect.left = cx / 2 - m_pLayout->margin;
			redraw_rect.right = m_ui_data.client_width / 2 + m_pLayout->margin;
			m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
		}
		if (cy < m_ui_data.client_height)	//如果界面高度变小了
		{
			//重新将绘图区域下方区域的矩形区域填充为对话框背景色
			redraw_rect = m_draw_rect;
			redraw_rect.top = cy - m_pLayout->margin;
			redraw_rect.bottom = cy;
			m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
		}
	}
	else if (m_ui_data.m_narrow_mode && cx < m_ui_data.client_width)	//在窄界面模式下，如果宽度变窄了
	{
		//重新将绘图区域右侧区域的矩形区域填充为对话框背景色
		redraw_rect = m_draw_rect;
		redraw_rect.left = cx - m_pLayout->margin;
		redraw_rect.right = cx;
		m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
	}
}

void CPlayerUIBase::UpdateRepeatModeToolTip()
{
	SetRepeatModeToolTipText();
	UpdateMouseToolTip(BTN_REPETEMODE, m_repeat_mode_tip);
}

void CPlayerUIBase::PreDrawInfo()
{
	//设置颜色
	if (theApp.m_app_setting_data.dark_mode)
	{
		m_colors.color_text = ColorTable::WHITE;
		m_colors.color_text_lable = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
		m_colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_back = GRAY(96);
		m_colors.color_lyric_back = theApp.m_app_setting_data.theme_color.dark3;
		m_colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.dark2;
		m_colors.color_spectrum = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.dark1;
		m_colors.color_button_back = theApp.m_app_setting_data.theme_color.dark3;

		m_colors.background_transparency = theApp.m_app_setting_data.background_transparency;
	}
	else
	{
		m_colors.color_text = theApp.m_app_setting_data.theme_color.dark2;
		m_colors.color_text_lable = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
		m_colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.dark1;
		m_colors.color_back = ColorTable::WHITE;
		m_colors.color_lyric_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_spectrum = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_button_back = theApp.m_app_setting_data.theme_color.light2;

		m_colors.background_transparency = theApp.m_app_setting_data.background_transparency;
	}

	//if (m_repeat_mode_tip.IsEmpty())
	SetRepeatModeToolTipText();

	//计算文本高度
	m_pDC->SelectObject(&m_ui_data.lyric_font);
	m_lyric_text_height = m_pDC->GetTextExtent(L"文").cy;	//根据当前的字体设置计算文本的高度
}

void CPlayerUIBase::DrawLyricTextMultiLine(CRect lyric_area, bool midi_lyric)
{
	int lyric_height = m_lyric_text_height + theApp.m_app_setting_data.lyric_line_space;			//文本高度加上行间距
	int lyric_height2 = lyric_height * 2 + theApp.m_app_setting_data.lyric_line_space;		//包含翻译的歌词高度

	m_draw.SetFont(&m_ui_data.lyric_font);
	if (midi_lyric)
	{
		wstring current_lyric{ theApp.m_player.GetMidiLyric() };
		m_draw.DrawWindowText(lyric_area, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
	}
	else if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_area, CCommon::LoadText(IDS_NO_LYRIC_INFO), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		//CRect arect{ lyric_area };		//一行歌词的矩形区域
		//arect.bottom = arect.top + lyric_height;
		//vector<CRect> rects(theApp.m_player.m_Lyrics.GetLyricCount() + 1, arect);
		//为每一句歌词创建一个矩形，保存在容器里
		vector<CRect> rects;
		int lyric_count = theApp.m_player.m_Lyrics.GetLyricCount() + 1;		//获取歌词数量（由于第一行歌词需要显示标题，所以这里要+1）
		for (int i{}; i < lyric_count; i++)
		{
			CRect arect{ lyric_area };
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				arect.bottom = arect.top + lyric_height2;
			else
				arect.bottom = arect.top + lyric_height;
			rects.push_back(arect);
		}
		int center_pos = (lyric_area.top + lyric_area.bottom) / 2;		//歌词区域的中心y坐标
		Time time{ theApp.m_player.GetCurrentPosition() };		//当前播放时间
		int lyric_index = theApp.m_player.m_Lyrics.GetLyricIndex(time) + 1;		//当前歌词的序号（歌词的第一句GetLyricIndex返回的是0，由于显示时第一句歌词要显示标题，所以这里要+1）
		int progress = theApp.m_player.m_Lyrics.GetLyricProgress(time);		//当前歌词进度（范围为0~1000）
		int y_progress;			//当前歌词在y轴上的进度
		if (!theApp.m_player.m_Lyrics.GetLyric(lyric_index).translate.empty() && m_ui_data.show_translate)
			y_progress = progress * lyric_height2 / 1000;
		else
			y_progress = progress * lyric_height / 1000;
		//int start_pos = center_pos - y_progress - (lyric_index + 1)*lyric_height;		//第1句歌词的起始y坐标
		//计算第1句歌词的起始y坐标
		//由于当前歌词需要显示在歌词区域的中心位置，因此从中心位置开始，减去当前歌词在Y轴上的进度
		//再依次减去之前每一句歌词的高度，即得到了第一句歌词的起始位置
		int start_pos;
		start_pos = center_pos - y_progress;
		for (int i{ lyric_index - 1 }; i >= 0; i--)
		{
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				start_pos -= lyric_height2;
			else
				start_pos -= lyric_height;
		}

		//依次绘制每一句歌词
		for (int i{}; i < rects.size(); i++)
		{
			//计算每一句歌词的位置
			if (i == 0)
				rects[i].MoveToY(start_pos);
			else
				rects[i].MoveToY(rects[i - 1].bottom);
			//绘制歌词文本
			if (!(rects[i] & lyric_area).IsRectEmpty())		//只有当一句歌词的矩形区域和歌词区域的矩形有交集时，才绘制歌词
			{
				//设置歌词文本和翻译文本的矩形区域
				CRect rect_text{ rects[i] };
				CRect rect_translate;
				if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				{
					rect_text.MoveToY(rect_text.top + theApp.m_app_setting_data.lyric_line_space);
					rect_text.bottom = rect_text.top + m_lyric_text_height;
					rect_translate = rect_text;
					rect_translate.MoveToY(rect_text.bottom + theApp.m_app_setting_data.lyric_line_space);
				}

				if (i == lyric_index)		//绘制正在播放的歌词
				{
					//绘制歌词文本
					m_draw.SetFont(&m_ui_data.lyric_font);
					if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
					else
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					//绘制翻译文本
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
					{
						m_draw.SetFont(&m_ui_data.lyric_translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					}
				}
				else		//绘制非正在播放的歌词
				{
					//绘制歌词文本
					m_draw.SetFont(&m_ui_data.lyric_font);
					m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					//绘制翻译文本
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
					{
						m_draw.SetFont(&m_ui_data.lyric_translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					}
				}
			}
		}
	}

}

void CPlayerUIBase::DrawLyricTextSingleLine(CRect rect, bool midi_lyric)
{
	m_draw.SetFont(&m_ui_data.lyric_font);
	if (midi_lyric)
	{
		wstring current_lyric{ theApp.m_player.GetMidiLyric() };
		m_draw.DrawWindowText(rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
	}
	else if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(rect, CCommon::LoadText(IDS_NO_LYRIC_INFO), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		CRect lyric_rect = rect;
		CLyrics::Lyric current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };	//获取当歌词
		if (current_lyric.text.empty())		//如果当前歌词为空白，就显示为省略号
			current_lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
		int progress{ theApp.m_player.m_Lyrics.GetLyricProgress(Time(theApp.m_player.GetCurrentPosition())) };		//获取当前歌词进度（范围为0~1000）

		if ((!theApp.m_player.m_Lyrics.IsTranslated() || !m_ui_data.show_translate) && rect.Height() > static_cast<int>(m_lyric_text_height*1.73))
		{
			wstring next_lyric_text = theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 1).text;
			if (next_lyric_text.empty())
				next_lyric_text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
			DrawLyricDoubleLine(lyric_rect, current_lyric.text.c_str(), next_lyric_text.c_str(), progress);
		}
		else
		{
			if (m_ui_data.show_translate && !current_lyric.translate.empty() && rect.Height() > static_cast<int>(m_lyric_text_height*1.73))
			{
				lyric_rect.bottom = lyric_rect.top + rect.Height() / 2;
				CRect translate_rect = lyric_rect;
				translate_rect.MoveToY(lyric_rect.bottom);

				m_draw.SetFont(&m_ui_data.lyric_translate_font);
				m_draw.DrawWindowText(translate_rect, current_lyric.translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
			}

			m_draw.SetFont(&m_ui_data.lyric_font);
			if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
			else
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
		}

		m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	}

}

void CPlayerUIBase::DrawSongInfo(CRect rect, bool reset)
{
	wchar_t buff[64];
	if (theApp.m_player.m_loading)
	{
		static CDrawCommon::ScrollInfo scroll_info0;
		CString info;
		info = CCommon::LoadTextFormat(IDS_PLAYLIST_INIT_INFO, { theApp.m_player.GetSongNum(), theApp.m_player.m_thread_info.process_percent });
		m_draw.DrawScrollText(rect, info, m_colors.color_text, theApp.DPI(1.5), false, scroll_info0, reset);
	}
	else
	{
		//绘制播放状态
		CString play_state_str = theApp.m_player.GetPlayingState().c_str();
		CRect rc_tmp{ rect };
		rc_tmp.right = rc_tmp.left + m_draw.GetDC()->GetTextExtent(play_state_str).cx - theApp.DPI(4);
		m_draw.DrawWindowText(rc_tmp, play_state_str, m_colors.color_text_lable);

		//绘制歌曲序号
		rc_tmp.MoveToX(rc_tmp.right);
		rc_tmp.right = rc_tmp.left + theApp.DPI(30);
		swprintf_s(buff, sizeof(buff) / 2, L"%.3d", theApp.m_player.GetIndex() + 1);
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text_2);

		//绘制文件名
		rc_tmp.MoveToX(rc_tmp.right);
		rc_tmp.right = rect.right;
		static CDrawCommon::ScrollInfo scroll_info1;
		m_draw.DrawScrollText(rc_tmp, theApp.m_player.GetFileName().c_str(), m_colors.color_text, theApp.DPI(1.5), false, scroll_info1, reset);
	}
}

void CPlayerUIBase::DrawControlBar(bool draw_background, CRect rect, bool draw_translate_button, UIData* pUIData)
{
	//绘制背景
	if (draw_background)
		m_draw.FillAlphaRect(rect, m_colors.color_control_bar_back, ALPHA_CHG(m_colors.background_transparency));
	else
		m_draw.FillRect(rect, m_colors.color_control_bar_back);

	CRect rc_tmp = rect;

	//绘制循环模式
	rc_tmp.right = rect.left + rect.Height();
	CRect rc_repeat_mode = rc_tmp;
	rc_repeat_mode.DeflateRect(theApp.DPI(2), theApp.DPI(2));
	m_draw.SetDrawArea(rc_repeat_mode);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(m_colors.background_transparency);
	else
		alpha = 255;
	if (m_buttons[BTN_REPETEMODE].hover)
		m_draw.FillAlphaRect(rc_repeat_mode, m_colors.color_text_2, alpha);
	//else if (!theApp.m_app_setting_data.dark_mode)
	//	m_draw.FillAlphaRect(rc_repeat_mode, m_colors.color_button_back, alpha);

	m_buttons[BTN_REPETEMODE].rect = DrawAreaToClient(rc_repeat_mode, m_draw_rect);

	rc_repeat_mode = rc_tmp;
	rc_repeat_mode.DeflateRect(theApp.DPI(4), theApp.DPI(4));

	switch (theApp.m_player.GetRepeatMode())
	{
	case RepeatMode::RM_PLAY_ORDER:
		m_draw.DrawIcon(theApp.m_play_oder_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
		break;
	case RepeatMode::RM_LOOP_PLAYLIST:
		m_draw.DrawIcon(theApp.m_loop_playlist_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
		break;
	case RepeatMode::RM_LOOP_TRACK:
		m_draw.DrawIcon(theApp.m_loop_track_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
		break;
	case RepeatMode::RM_PLAY_SHUFFLE:
		m_draw.DrawIcon(theApp.m_play_shuffle_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
		break;
	}
	

	//绘制设置按钮
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_buttons[BTN_SETTING], theApp.m_setting_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//绘制均衡器按钮
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_buttons[BTN_EQ], theApp.m_eq_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//绘制切换界面按钮
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_buttons[BTN_SKIN], theApp.m_skin_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//绘制迷你模式按钮
	if (rect.Width() >= theApp.DPI(190))
	{
		rc_tmp.MoveToX(rc_tmp.right);
		DrawUIButton(rc_tmp, m_buttons[BTN_MINI], theApp.m_mini_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);
	}
	else
	{
		m_buttons[BTN_MINI].rect = CRect();
	}

	//绘制翻译按钮
	if (draw_translate_button && rect.Width() >= theApp.DPI(214))
	{
		rc_tmp.MoveToX(rc_tmp.right);
		CRect translate_rect = rc_tmp;
		translate_rect.DeflateRect(theApp.DPI(2), theApp.DPI(2));
		m_buttons[BTN_TRANSLATE].enable = theApp.m_player.m_Lyrics.IsTranslated();
		if (m_buttons[BTN_TRANSLATE].enable)
		{
			BYTE alpha;
			if (draw_background)
				alpha = ALPHA_CHG(m_colors.background_transparency);
			else
				alpha = 255;
			if (m_buttons[BTN_TRANSLATE].hover)
				m_draw.FillAlphaRect(translate_rect, m_colors.color_text_2, alpha);
			else if (pUIData!=nullptr && pUIData->show_translate)
				m_draw.FillAlphaRect(translate_rect, m_colors.color_button_back, alpha);
			m_draw.DrawWindowText(translate_rect, CCommon::LoadText(IDS_TRAS), m_colors.color_text, Alignment::CENTER);
		}
		else
		{
			m_draw.DrawWindowText(translate_rect, CCommon::LoadText(IDS_TRAS), GRAY(200), Alignment::CENTER);
		}
		m_buttons[BTN_TRANSLATE].rect = DrawAreaToClient(translate_rect, m_draw_rect);

	}
	else
	{
		m_buttons[BTN_TRANSLATE].rect = CRect();
	}

	//显示<<<<
	rc_tmp.left = rc_tmp.right = rect.right;
	if (rect.Width() >= theApp.DPI(265))
	{
		int progress;
		Time time{ theApp.m_player.GetCurrentPosition() };
		if (theApp.m_player.IsMidi())
		{
			////progress = (theApp.m_player.GetMidiInfo().midi_position % 16 + 1) *1000 / 16;
			//if (theApp.m_player.GetMidiInfo().tempo == 0)
			//	progress = 0;
			//else
			//	progress = (time.time2int() * 1000 / theApp.m_player.GetMidiInfo().tempo % 4 + 1) * 250;
			progress = (theApp.m_player.GetMidiInfo().midi_position % 4 + 1) * 250;
		}
		else
		{
			progress = (time.sec % 4 * 1000 + time.msec) / 4;
		}
		rc_tmp.right = rect.right;
		rc_tmp.left = rc_tmp.right - theApp.DPI(44);
		m_draw.DrawWindowText(rc_tmp, _T("<<<<"), m_colors.color_text, m_colors.color_text_2, progress, false);
	}


	//显示音量
	wchar_t buff[64];
	rc_tmp.right = rc_tmp.left;
	rc_tmp.left = rc_tmp.right - theApp.DPI(72);
	swprintf_s(buff, CCommon::LoadText(IDS_VOLUME, _T(": %d%%")), theApp.m_player.GetVolume());
	if (m_buttons[BTN_VOLUME].hover)		//鼠标指向音量区域时，以另外一种颜色显示
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text_heighlight);
	else
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text);
	//设置音量调整按钮的位置
	m_buttons[BTN_VOLUME].rect = DrawAreaToClient(rc_tmp, m_draw_rect);
	m_buttons[BTN_VOLUME].rect.DeflateRect(0, theApp.DPI(4));
	m_buttons[BTN_VOLUME].rect.right -= theApp.DPI(12);
	m_volume_down_rect = m_buttons[BTN_VOLUME].rect;
	m_volume_down_rect.bottom += theApp.DPI(4);
	m_volume_down_rect.MoveToY(m_buttons[BTN_VOLUME].rect.bottom);
	m_volume_down_rect.right = m_buttons[BTN_VOLUME].rect.left + m_buttons[BTN_VOLUME].rect.Width() / 2;
	m_volume_up_rect = m_volume_down_rect;
	m_volume_up_rect.MoveToX(m_volume_down_rect.right);


}

CRect CPlayerUIBase::DrawAreaToClient(CRect rect, CRect draw_area)
{
	rect.MoveToXY(rect.left + draw_area.left, rect.top + draw_area.top);
	return rect;
}

CRect CPlayerUIBase::ClientAreaToDraw(CRect rect, CRect draw_area)
{
	rect.MoveToXY(rect.left - draw_area.left, rect.top - draw_area.top);
	return rect;
}

void CPlayerUIBase::DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	m_draw.SetFont(&m_ui_data.lyric_font);
	static bool swap;
	static int last_progress;
	if (last_progress > progress)		//如果当前的歌词进度比上次的小，说明歌词切换到了下一句
	{
		swap = !swap;
	}
	last_progress = progress;


	CRect up_rect{ rect }, down_rect{ rect };		//上半部分和下半部分歌词的矩形区域
	up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
	down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
	//根据下一句歌词的文本计算需要的宽度，从而实现下一行歌词右对齐
	m_draw.GetDC()->SelectObject(&m_ui_data.lyric_font);
	int width;
	if (!swap)
		width = m_draw.GetDC()->GetTextExtent(next_lyric).cx;
	else
		width = m_draw.GetDC()->GetTextExtent(lyric).cx;
	if (width < rect.Width())
		down_rect.left = down_rect.right - width;

	COLORREF color1, color2;
	if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
	{
		color1 = m_colors.color_text;
		color2 = m_colors.color_text_2;
	}
	else
	{
		color1 = color2 = m_colors.color_text;
	}

	if (!swap)
	{
		m_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false);
		m_draw.DrawWindowText(down_rect, next_lyric, m_colors.color_text_2);
	}
	else
	{
		m_draw.DrawWindowText(up_rect, next_lyric, m_colors.color_text_2);
		m_draw.DrawWindowText(down_rect, lyric, color1, color2, progress, false);
	}

}

void CPlayerUIBase::DrawUIButton(CRect rect, UIButton & btn, HICON icon, bool draw_background)
{
	CRect rc_tmp = rect;
	rc_tmp.DeflateRect(theApp.DPI(2), theApp.DPI(2));
	m_draw.SetDrawArea(rc_tmp);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(m_colors.background_transparency);
	else
		alpha = 255;
	if (btn.hover)
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_text_2, alpha);
	//else if (!theApp.m_app_setting_data.dark_mode)
	//	m_draw.FillAlphaRect(rc_tmp, m_colors.color_button_back, alpha);

	btn.rect = DrawAreaToClient(rc_tmp, m_draw_rect);

	rc_tmp = rect;
	rc_tmp.DeflateRect(theApp.DPI(4), theApp.DPI(4));
	m_draw.DrawIcon(icon, rc_tmp.TopLeft(), rc_tmp.Size());

}

void CPlayerUIBase::SetRepeatModeToolTipText()
{
	m_repeat_mode_tip = CCommon::LoadText(IDS_REPEAT_MODE, _T(": "));
	switch (theApp.m_player.GetRepeatMode())
	{
	case RepeatMode::RM_PLAY_ORDER:
		m_repeat_mode_tip += CCommon::LoadText(IDS_PLAY_ODER);
		break;
	case RepeatMode::RM_LOOP_PLAYLIST:
		m_repeat_mode_tip += CCommon::LoadText(IDS_LOOP_PLAYLIST);
		break;
	case RepeatMode::RM_LOOP_TRACK:
		m_repeat_mode_tip += CCommon::LoadText(IDS_LOOP_TRACK);
		break;
	case RepeatMode::RM_PLAY_SHUFFLE:
		m_repeat_mode_tip += CCommon::LoadText(IDS_PLAY_SHUFFLE);
		break;
	}
}

void CPlayerUIBase::DrawVolumnAdjBtn(bool draw_background)
{
	if (m_show_volume_adj)
	{
		CRect volume_down_rect = ClientAreaToDraw(m_volume_down_rect, m_draw_rect);
		CRect volume_up_rect = ClientAreaToDraw(m_volume_up_rect, m_draw_rect);

		if (draw_background)
		{
			m_draw.FillAlphaRect(volume_down_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
			m_draw.FillAlphaRect(volume_up_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
		}
		else
		{
			m_draw.FillRect(volume_down_rect, m_colors.color_text_2);
			m_draw.FillRect(volume_up_rect, m_colors.color_text_2);
		}
		m_draw.DrawWindowText(volume_down_rect, L"-", ColorTable::WHITE, Alignment::CENTER);
		m_draw.DrawWindowText(volume_up_rect, L"+", ColorTable::WHITE, Alignment::CENTER);
	}
}

//void CPlayerUIBase::AddMouseToolTip(BtnKey btn, LPCTSTR str)
//{
//	m_tool_tip->AddTool(theApp.m_pMainWnd, str, m_buttons[btn].rect, btn + 1);
//}
//
//void CPlayerUIBase::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
//{
//	m_tool_tip->UpdateTipText(str, theApp.m_pMainWnd, btn + 1);
//}

void CPlayerUIBase::AddToolTips()
{
	AddMouseToolTip(BTN_REPETEMODE, m_repeat_mode_tip);
	AddMouseToolTip(BTN_TRANSLATE, CCommon::LoadText(IDS_SHOW_LYRIC_TRANSLATION));
	AddMouseToolTip(BTN_VOLUME, CCommon::LoadText(IDS_MOUSE_WHEEL_ADJUST_VOLUME));
	AddMouseToolTip(BTN_SKIN, CCommon::LoadText(IDS_SWITCH_UI));
	AddMouseToolTip(BTN_EQ, CCommon::LoadText(IDS_SOUND_EFFECT_SETTING));
	AddMouseToolTip(BTN_SETTING, CCommon::LoadText(IDS_SETTINGS));
	AddMouseToolTip(BTN_MINI, CCommon::LoadText(IDS_MINI_MODE));
}

