
// MusicPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SupportedFormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	CToolTipCtrl m_tool_tip;		//鼠标指向时的工具提示

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickGithubSyslink(NMHDR *pNMHDR, LRESULT *pResult);
};

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CAboutDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_GITHUB_SYSLINK, &CAboutDlg::OnNMClickGithubSyslink)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString version_info;
	GetDlgItemText(IDC_STATIC_VERSION, version_info);
	version_info.Replace(_T("<version>"), VERSION);
#ifdef COMPILE_IN_WIN_XP
	version_info += _T(" (For WinXP)");
#endif // COMPILE_FOR_WINXP
	SetDlgItemText(IDC_STATIC_VERSION, version_info);

	//设置最后编译日期
	CString temp_str;
	GetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);
	temp_str.Replace(_T("<compile_date>"), COMPILE_DATE);
	SetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);

	m_tool_tip.Create(this);
	m_tool_tip.AddTool(GetDlgItem(IDC_SYSLINK1), CCommon::LoadText(IDS_SEND_EMAIL_TO_ATHOUR,_T("\r\nmailto:zhongyang219@hotmail.com")));
	m_tool_tip.AddTool(GetDlgItem(IDC_GITHUB_SYSLINK), CCommon::LoadText(IDS_GOTO_GITHUB, _T("\r\nhttps://github.com/zhongyang219/MusicPlayer2")));
	m_tool_tip.SetDelayTime(300);	//设置延迟
	m_tool_tip.SetMaxTipWidth(theApp.DPI(400));

	//if (theApp.m_is_windows10)
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Windows10"));
	//else
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Not Windows10"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//点击了“联系作者”
	ShellExecute(NULL, _T("open"), _T("mailto:zhongyang219@hotmail.com"), NULL, NULL, SW_SHOW);	//打开超链接
	*pResult = 0;
}

void CAboutDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//点击了“检查更新”
	theApp.CheckUpdate(true);
	*pResult = 0;
}

void CAboutDlg::OnNMClickGithubSyslink(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), _T("https://github.com/zhongyang219/MusicPlayer2/"), NULL, NULL, SW_SHOW);	//打开超链接
	*pResult = 0;
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
		m_tool_tip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}


// CMusicPlayerDlg 对话框

const UINT WM_TASKBARCREATED{ ::RegisterWindowMessage(_T("TaskbarCreated")) };	//注册任务栏建立的消息

CMusicPlayerDlg::CMusicPlayerDlg(wstring cmdLine, CWnd* pParent /*=NULL*/)
	: m_cmdLine{cmdLine}, CDialog(IDD_MUSICPLAYER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMusicPlayerDlg::~CMusicPlayerDlg()
{
	CCommon::DeleteModelessDialog(m_pLyricEdit);
	CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
	CCommon::DeleteModelessDialog(m_pSetPathDlg);
	CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
}

void CMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
	DDX_Control(pDX, IDC_PLAYLIST_LIST, m_playlist_list);
	DDX_Control(pDX, IDC_PATH_STATIC, m_path_static);
	DDX_Control(pDX, IDC_PATH_EDIT, m_path_edit);
	DDX_Control(pDX, IDC_PLAY_PAUSE, m_play_pause_button);
	DDX_Control(pDX, IDC_STOP, m_stop_button);
	DDX_Control(pDX, IDC_PREVIOUS, m_previous_button);
	DDX_Control(pDX, IDC_NEXT, m_next_button);
	DDX_Control(pDX, IDC_PROGRESS_STATIC, m_progress_bar);
	DDX_Control(pDX, IDC_TIME_STATIC, m_time_static);
	DDX_Control(pDX, ID_SET_PATH, m_set_path_button);
	DDX_Control(pDX, IDC_SEARCH_EDIT, m_search_edit);
	DDX_Control(pDX, IDC_CLEAR_SEARCH_BUTTON, m_clear_search_button);
}

BEGIN_MESSAGE_MAP(CMusicPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_PLAY_PAUSE, &CMusicPlayerDlg::OnPlayPause)
	ON_COMMAND(ID_STOP, &CMusicPlayerDlg::OnStop)
	ON_COMMAND(ID_PREVIOUS, &CMusicPlayerDlg::OnPrevious)
	ON_COMMAND(ID_NEXT, &CMusicPlayerDlg::OnNext)
	ON_COMMAND(ID_REW, &CMusicPlayerDlg::OnRew)
	ON_COMMAND(ID_FF, &CMusicPlayerDlg::OnFF)
	ON_COMMAND(ID_SET_PATH, &CMusicPlayerDlg::OnSetPath)
	ON_COMMAND(ID_FIND, &CMusicPlayerDlg::OnFind)
	ON_COMMAND(ID_EXPLORE_PATH, &CMusicPlayerDlg::OnExplorePath)
	ON_WM_DESTROY()
	ON_COMMAND(ID_APP_ABOUT, &CMusicPlayerDlg::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, &CMusicPlayerDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN_FOLDER, &CMusicPlayerDlg::OnFileOpenFolder)
	ON_WM_DROPFILES()
	ON_WM_INITMENU()
	ON_COMMAND(ID_PLAY_ORDER, &CMusicPlayerDlg::OnPlayOrder)
	ON_COMMAND(ID_PLAY_SHUFFLE, &CMusicPlayerDlg::OnPlayShuffle)
	ON_COMMAND(ID_LOOP_PLAYLIST, &CMusicPlayerDlg::OnLoopPlaylist)
	ON_COMMAND(ID_LOOP_TRACK, &CMusicPlayerDlg::OnLoopTrack)
	ON_WM_MOUSEWHEEL()
	//ON_BN_CLICKED(IDC_VOLUME_UP, &CMusicPlayerDlg::OnBnClickedVolumeUp)
	//ON_BN_CLICKED(IDC_VOLUME_DOWN, &CMusicPlayerDlg::OnBnClickedVolumeDown)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMDblclkPlaylistList)
	//ON_COMMAND(ID_REFRESH_PLAYLIST, &CMusicPlayerDlg::OnRefreshPlaylist)
	ON_COMMAND(ID_OPTION_SETTINGS, &CMusicPlayerDlg::OnOptionSettings)
	ON_COMMAND(ID_RELOAD_PLAYLIST, &CMusicPlayerDlg::OnReloadPlaylist)
	ON_NOTIFY(NM_RCLICK, IDC_PLAYLIST_LIST, &CMusicPlayerDlg::OnNMRClickPlaylistList)
	ON_COMMAND(ID_PLAY_ITEM, &CMusicPlayerDlg::OnPlayItem)
	ON_COMMAND(ID_ITEM_PROPERTY, &CMusicPlayerDlg::OnItemProperty)
	//ON_COMMAND(ID_REMOVE_FROM_PLAYLIST, &CMusicPlayerDlg::OnRemoveFromPlaylist)
	//ON_COMMAND(ID_CLEAR_PLAYLIST, &CMusicPlayerDlg::OnClearPlaylist)
	ON_COMMAND(ID_EXPLORE_TRACK, &CMusicPlayerDlg::OnExploreTrack)
	ON_WM_HOTKEY()
	ON_STN_CLICKED(IDC_PROGRESS_STATIC, &CMusicPlayerDlg::OnStnClickedProgressStatic)
	ON_COMMAND(ID_RE_INI_BASS, &CMusicPlayerDlg::OnReIniBass)
	ON_COMMAND(ID_SORT_BY_FILE, &CMusicPlayerDlg::OnSortByFile)
	ON_COMMAND(ID_SORT_BY_TITLE, &CMusicPlayerDlg::OnSortByTitle)
	ON_COMMAND(ID_SORT_BY_ARTIST, &CMusicPlayerDlg::OnSortByArtist)
	ON_COMMAND(ID_SORT_BY_ALBUM, &CMusicPlayerDlg::OnSortByAlbum)
	ON_COMMAND(ID_SORT_BY_TRACK, &CMusicPlayerDlg::OnSortByTrack)
	ON_COMMAND(ID_DELETE_FROM_DISK, &CMusicPlayerDlg::OnDeleteFromDisk)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, &CMusicPlayerDlg::OnTaskbarcreated)
	ON_COMMAND(ID_DISP_FILE_NAME, &CMusicPlayerDlg::OnDispFileName)
	ON_COMMAND(ID_DISP_TITLE, &CMusicPlayerDlg::OnDispTitle)
	ON_COMMAND(ID_DISP_ARTIST_TITLE, &CMusicPlayerDlg::OnDispArtistTitle)
	ON_COMMAND(ID_DISP_TITLE_ARTIST, &CMusicPlayerDlg::OnDispTitleArtist)
	ON_COMMAND(ID_MINI_MODE, &CMusicPlayerDlg::OnMiniMode)
	ON_BN_CLICKED(IDC_STOP, &CMusicPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PREVIOUS, &CMusicPlayerDlg::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_PLAY_PAUSE, &CMusicPlayerDlg::OnBnClickedPlayPause)
	ON_BN_CLICKED(IDC_NEXT, &CMusicPlayerDlg::OnBnClickedNext)
//	ON_WM_MOVE()
	ON_COMMAND(ID_RELOAD_LYRIC, &CMusicPlayerDlg::OnReloadLyric)
	ON_COMMAND(ID_SONG_INFO, &CMusicPlayerDlg::OnSongInfo)
	ON_COMMAND(ID_COPY_CURRENT_LYRIC, &CMusicPlayerDlg::OnCopyCurrentLyric)
	ON_COMMAND(ID_COPY_ALL_LYRIC, &CMusicPlayerDlg::OnCopyAllLyric)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_LYRIC_FORWARD, &CMusicPlayerDlg::OnLyricForward)
	ON_COMMAND(ID_LYRIC_DELAY, &CMusicPlayerDlg::OnLyricDelay)
	ON_COMMAND(ID_SAVE_MODIFIED_LYRIC, &CMusicPlayerDlg::OnSaveModifiedLyric)
	ON_COMMAND(ID_EDIT_LYRIC, &CMusicPlayerDlg::OnEditLyric)
	ON_COMMAND(ID_DOWNLOAD_LYRIC, &CMusicPlayerDlg::OnDownloadLyric)
	ON_COMMAND(ID_LYRIC_BATCH_DOWNLOAD, &CMusicPlayerDlg::OnLyricBatchDownload)
	ON_COMMAND(ID_DELETE_LYRIC, &CMusicPlayerDlg::OnDeleteLyric)
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_PLAYLIST_INI_COMPLATE, &CMusicPlayerDlg::OnPlaylistIniComplate)
	ON_MESSAGE(WM_SET_TITLE, &CMusicPlayerDlg::OnSetTitle)
	ON_COMMAND(ID_EQUALIZER, &CMusicPlayerDlg::OnEqualizer)
	ON_COMMAND(ID_EXPLORE_ONLINE, &CMusicPlayerDlg::OnExploreOnline)
	ON_MESSAGE(WM_PLAYLIST_INI_START, &CMusicPlayerDlg::OnPlaylistIniStart)
	ON_COMMAND(ID_BROWSE_LYRIC, &CMusicPlayerDlg::OnBrowseLyric)
	ON_COMMAND(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, &CMusicPlayerDlg::OnTranslateToSimplifiedChinese)
	ON_COMMAND(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, &CMusicPlayerDlg::OnTranslateToTranditionalChinese)
	ON_COMMAND(ID_ALBUM_COVER_SAVE_AS, &CMusicPlayerDlg::OnAlbumCoverSaveAs)
	ON_MESSAGE(WM_PATH_SELECTED, &CMusicPlayerDlg::OnPathSelected)
	ON_MESSAGE(WM_CONNOT_PLAY_WARNING, &CMusicPlayerDlg::OnConnotPlayWarning)
	ON_EN_CHANGE(IDC_SEARCH_EDIT, &CMusicPlayerDlg::OnEnChangeSearchEdit)
	ON_BN_CLICKED(IDC_CLEAR_SEARCH_BUTTON, &CMusicPlayerDlg::OnBnClickedClearSearchButton)
	ON_COMMAND(ID_DOWNLOAD_ALBUM_COVER, &CMusicPlayerDlg::OnDownloadAlbumCover)
	ON_MESSAGE(WM_MUSIC_STREAM_OPENED, &CMusicPlayerDlg::OnMusicStreamOpened)
	ON_COMMAND(ID_CURRENT_EXPLORE_ONLINE, &CMusicPlayerDlg::OnCurrentExploreOnline)
	ON_COMMAND(ID_DELETE_ALBUM_COVER, &CMusicPlayerDlg::OnDeleteAlbumCover)
	ON_COMMAND(ID_COPY_FILE_TO, &CMusicPlayerDlg::OnCopyFileTo)
	ON_COMMAND(ID_MOVE_FILE_TO, &CMusicPlayerDlg::OnMoveFileTo)
	ON_MESSAGE(WM_OPEN_FILE_COMMAND_LINE, &CMusicPlayerDlg::OnOpenFileCommandLine)
	ON_COMMAND(ID_FORMAT_CONVERT, &CMusicPlayerDlg::OnFormatConvert)
	ON_COMMAND(ID_FORMAT_CONVERT1, &CMusicPlayerDlg::OnFormatConvert1)
	ON_MESSAGE(WM_SETTINGS_APPLIED, &CMusicPlayerDlg::OnSettingsApplied)
	ON_COMMAND(ID_RECORDER, &CMusicPlayerDlg::OnRecorder)
	ON_MESSAGE(WM_ALBUM_COVER_DOWNLOAD_COMPLETE, &CMusicPlayerDlg::OnAlbumCoverDownloadComplete)
	ON_WM_DWMCOLORIZATIONCOLORCHANGED()
	ON_COMMAND(ID_SUPPORTED_FORMAT, &CMusicPlayerDlg::OnSupportedFormat)
	ON_COMMAND(ID_SWITCH_UI, &CMusicPlayerDlg::OnSwitchUi)
	ON_COMMAND(ID_VOLUME_UP, &CMusicPlayerDlg::OnVolumeUp)
	ON_COMMAND(ID_VOLUME_DOWN, &CMusicPlayerDlg::OnVolumeDown)
	ON_MESSAGE(MY_WM_NOTIFYICON, &CMusicPlayerDlg::OnNotifyicon)
	ON_WM_CLOSE()
	ON_COMMAND(ID_MENU_EXIT, &CMusicPlayerDlg::OnMenuExit)
	ON_COMMAND(ID_MINIMODE_RESTORE, &CMusicPlayerDlg::OnMinimodeRestore)
		ON_WM_APPCOMMAND()
		END_MESSAGE_MAP()


// CMusicPlayerDlg 消息处理程序

void CMusicPlayerDlg::SaveConfig()
{
	CIniHelper ini;
	ini.SetPath(theApp.m_config_path);

	ini.WriteInt(L"config", L"window_width", m_window_width);
	ini.WriteInt(L"config", L"window_hight", m_window_height);
	ini.WriteInt(L"config", L"transparency", theApp.m_app_setting_data.window_transparency);
	ini.WriteBool(L"config", L"narrow_mode", m_ui_data.m_narrow_mode);
	ini.WriteBool(L"config", L"show_translate", m_ui_data.show_translate);

	ini.WriteInt(L"config", L"theme_color", theApp.m_app_setting_data.theme_color.original_color);
	ini.WriteBool(L"config", L"theme_color_follow_system", theApp.m_app_setting_data.theme_color_follow_system);
	ini.WriteInt(L"config", L"playlist_display_format", static_cast<int>(m_display_format));
	ini.WriteBool(L"config", L"show_lyric_in_cortana", theApp.m_lyric_setting_data.show_lyric_in_cortana);
	ini.WriteBool(L"config", L"save_lyric_in_offset", theApp.m_lyric_setting_data.save_lyric_in_offset);
	ini.WriteString(L"config",L"font", theApp.m_app_setting_data.lyric_font_name);
	ini.WriteInt(L"config", L"font_size", theApp.m_app_setting_data.lyric_font_size);
	ini.WriteInt(L"config", L"lyric_line_space", theApp.m_app_setting_data.lyric_line_space);
	ini.WriteInt(L"config", L"spectrum_height", theApp.m_app_setting_data.sprctrum_height);
	ini.WriteBool(L"config", L"cortana_lyric_double_line", theApp.m_lyric_setting_data.cortana_lyric_double_line);
	ini.WriteBool(L"config", L"show_spectrum", theApp.m_app_setting_data.show_spectrum);
	ini.WriteBool(L"config", L"show_album_cover", theApp.m_app_setting_data.show_album_cover);
	ini.WriteInt(L"config", L"album_cover_fit", static_cast<int>(theApp.m_app_setting_data.album_cover_fit));
	ini.WriteBool(L"config", L"album_cover_as_background", theApp.m_app_setting_data.album_cover_as_background);
	ini.WriteBool(L"config", L"cortana_show_album_cover", theApp.m_lyric_setting_data.cortana_show_album_cover);
	ini.WriteBool(L"config", L"cortana_icon_beat", theApp.m_lyric_setting_data.cortana_icon_beat);
	ini.WriteBool(L"config", L"background_gauss_blur", theApp.m_app_setting_data.background_gauss_blur);
	ini.WriteInt(L"config", L"gauss_blur_radius", theApp.m_app_setting_data.gauss_blur_radius);
	ini.WriteBool(L"config", L"lyric_background", theApp.m_app_setting_data.lyric_background);
	ini.WriteBool(L"config", L"dark_mode", theApp.m_app_setting_data.dark_mode);

	ini.WriteInt(L"config", L"background_transparency", theApp.m_app_setting_data.background_transparency);
	ini.WriteBool(L"config", L"use_out_image", theApp.m_app_setting_data.use_out_image);
	ini.WriteInt(L"config", L"volum_step", theApp.m_nc_setting_data.volum_step);
	ini.WriteInt(L"config", L"mouse_volum_step", theApp.m_nc_setting_data.mouse_volum_step);
	ini.WriteInt(L"config", L"cortana_back_color", theApp.m_lyric_setting_data.cortana_color);
	ini.WriteInt(L"config", L"volume_map", theApp.m_nc_setting_data.volume_map);
	ini.WriteBool(L"config", L"show_cover_tip", theApp.m_nc_setting_data.show_cover_tip);
	ini.WriteBool(L"other", L"no_sf2_warning", theApp.m_nc_setting_data.no_sf2_warning);

	ini.WriteBool(L"general", L"id3v2_first", theApp.m_general_setting_data.id3v2_first);
	ini.WriteBool(L"general", L"auto_download_lyric", theApp.m_general_setting_data.auto_download_lyric);
	ini.WriteBool(L"general", L"auto_download_album_cover", theApp.m_general_setting_data.auto_download_album_cover);
	ini.WriteBool(L"general", L"auto_download_only_tag_full", theApp.m_general_setting_data.auto_download_only_tag_full);
	ini.WriteString(L"general", L"sf2_path", theApp.m_general_setting_data.sf2_path);
	ini.WriteBool(L"general", L"midi_use_inner_lyric", theApp.m_general_setting_data.midi_use_inner_lyric);
	ini.WriteBool(L"general", L"minimize_to_notify_icon", theApp.m_general_setting_data.minimize_to_notify_icon);

	ini.WriteBool(L"config", L"stop_when_error", theApp.m_play_setting_data.stop_when_error);
	ini.WriteBool(L"config", L"auto_play_when_start", theApp.m_play_setting_data.auto_play_when_start);
	ini.WriteBool(L"config", L"show_taskbar_progress", theApp.m_play_setting_data.show_taskbar_progress);
	ini.WriteBool(L"config", L"show_playstate_icon", theApp.m_play_setting_data.show_playstate_icon);
	ini.WriteString(L"config", L"output_device", theApp.m_play_setting_data.output_device);

	int ui_selected;
	if (m_pUI == &m_ui)
		ui_selected = 0;
	else
		ui_selected = 1;
	ini.WriteInt(L"config", L"UI_selected", ui_selected);

	//保存热键设置
	ini.WriteBool(L"hot_key", L"hot_key_enable", theApp.m_hot_key_enable);

	wstring str;
	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_PLAY_PAUSE));
	ini.WriteString(L"hot_key", L"play_pause", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_STOP));
	ini.WriteString(L"hot_key", L"stop", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_FF));
	ini.WriteString(L"hot_key", L"fast_forward", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_REW));
	ini.WriteString(L"hot_key", L"rewind", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_PREVIOUS));
	ini.WriteString(L"hot_key", L"previous", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_NEXT));
	ini.WriteString(L"hot_key", L"next", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_VOLUME_UP));
	ini.WriteString(L"hot_key", L"volume_up", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_VOLUME_DOWN));
	ini.WriteString(L"hot_key", L"volume_down", str);

	str = CHotkeyManager::HotkeyToString(theApp.m_hot_key.GetHotKey(HK_EXIT));
	ini.WriteString(L"hot_key", L"exit", str);

}

void CMusicPlayerDlg::LoadConfig()
{
	CIniHelper ini;
	ini.SetPath(theApp.m_config_path);

	m_window_width = ini.GetInt(L"config", L"window_width", theApp.DPI(698));
	m_window_height = ini.GetInt(L"config", L"window_hight", theApp.DPI(565));
	theApp.m_app_setting_data.window_transparency = ini.GetInt(_T("config"), _T("transparency"), 100);
	m_ui_data.m_narrow_mode = ini.GetBool(_T("config"), _T("narrow_mode"), 0);
	m_ui_data.show_translate = ini.GetBool(_T("config"), _T("show_translate"), true);

	theApp.m_app_setting_data.theme_color.original_color = ini.GetInt(_T("config"), _T("theme_color"), 16760187);
	theApp.m_app_setting_data.theme_color_follow_system = ini.GetBool(_T("config"), _T("theme_color_follow_system"), 1);
	m_display_format = static_cast<DisplayFormat>(ini.GetInt(_T("config"), _T("playlist_display_format"), 2));
	theApp.m_lyric_setting_data.show_lyric_in_cortana = ini.GetBool(_T("config"), _T("show_lyric_in_cortana"), 0);
	theApp.m_lyric_setting_data.save_lyric_in_offset = ini.GetBool(_T("config"), _T("save_lyric_in_offset"), 0);
	theApp.m_app_setting_data.lyric_font_name = ini.GetString(L"config", L"font", CCommon::LoadText(IDS_DEFAULT_FONT));
	theApp.m_app_setting_data.lyric_font_size = ini.GetInt(L"config", L"font_size", 10);
	theApp.m_app_setting_data.lyric_line_space = ini.GetInt(L"config", L"lyric_line_space", 2);
	theApp.m_app_setting_data.sprctrum_height = ini.GetInt(L"config", L"spectrum_height", 100);
	theApp.m_lyric_setting_data.cortana_lyric_double_line = ini.GetBool(_T("config"), _T("cortana_lyric_double_line"), 0);
	theApp.m_app_setting_data.show_spectrum = ini.GetBool(_T("config"), _T("show_spectrum"), 1);
	theApp.m_app_setting_data.show_album_cover = ini.GetBool(_T("config"), _T("show_album_cover"), 1);
	theApp.m_app_setting_data.album_cover_fit = static_cast<CDrawCommon::StretchMode>(ini.GetInt(_T("config"), _T("album_cover_fit"), 1));
	theApp.m_app_setting_data.album_cover_as_background = ini.GetBool(_T("config"), _T("album_cover_as_background"), true);
	theApp.m_lyric_setting_data.cortana_show_album_cover = ini.GetBool(_T("config"), _T("cortana_show_album_cover"), 1);
	theApp.m_lyric_setting_data.cortana_icon_beat = ini.GetBool(_T("config"), _T("cortana_icon_beat"), 0);
	theApp.m_app_setting_data.background_gauss_blur = ini.GetBool(_T("config"), _T("background_gauss_blur"), true);
	theApp.m_app_setting_data.gauss_blur_radius = ini.GetInt(_T("config"), _T("gauss_blur_radius"), 60);
	theApp.m_app_setting_data.lyric_background = ini.GetBool(_T("config"), _T("lyric_background"), true);
	theApp.m_app_setting_data.dark_mode = ini.GetBool(_T("config"), _T("dark_mode"), false);

	theApp.m_app_setting_data.background_transparency = ini.GetInt(L"config", L"background_transparency", 80);
	theApp.m_app_setting_data.use_out_image = ini.GetBool(_T("config"), _T("use_out_image"), 0);
	theApp.m_nc_setting_data.volum_step = ini.GetInt(_T("config"), _T("volum_step"), 3);
	theApp.m_nc_setting_data.mouse_volum_step = ini.GetInt(_T("config"), _T("mouse_volum_step"), 2);
	theApp.m_lyric_setting_data.cortana_color = ini.GetInt(_T("config"), _T("cortana_back_color"), 0);
	theApp.m_nc_setting_data.volume_map = ini.GetInt(_T("config"), _T("volume_map"), 100);
	theApp.m_nc_setting_data.show_cover_tip = ini.GetBool(_T("config"), _T("show_cover_tip"), 0);
	theApp.m_nc_setting_data.no_sf2_warning = ini.GetInt(L"other", L"no_sf2_warning", true);

	theApp.m_general_setting_data.id3v2_first = ini.GetBool(_T("general"), _T("id3v2_first"), 1);
	theApp.m_general_setting_data.auto_download_lyric = ini.GetBool(_T("general"), _T("auto_download_lyric"), 1);
	theApp.m_general_setting_data.auto_download_album_cover = ini.GetBool(_T("general"), _T("auto_download_album_cover"), 1);
	theApp.m_general_setting_data.auto_download_only_tag_full = ini.GetBool(_T("general"), _T("auto_download_only_tag_full"), 1);
	theApp.m_general_setting_data.sf2_path = ini.GetString(L"general", L"sf2_path", L"");
	theApp.m_general_setting_data.midi_use_inner_lyric = ini.GetBool(_T("general"), _T("midi_use_inner_lyric"), 0);
	theApp.m_general_setting_data.minimize_to_notify_icon = ini.GetBool(L"general", L"minimize_to_notify_icon", false);

	theApp.m_play_setting_data.stop_when_error = ini.GetBool(_T("config"), _T("stop_when_error"), true);
	theApp.m_play_setting_data.auto_play_when_start = ini.GetBool(_T("config"), _T("auto_play_when_start"), false);
	theApp.m_play_setting_data.show_taskbar_progress = ini.GetBool(_T("config"), _T("show_taskbar_progress"), true);
	theApp.m_play_setting_data.show_playstate_icon = ini.GetBool(_T("config"), _T("show_playstate_icon"), true);
	theApp.m_play_setting_data.output_device = ini.GetString(L"config", L"output_device", L"");

	int ui_selected = ini.GetInt(L"config", L"UI_selected", 1);
	if (ui_selected == 0)
		m_pUI = &m_ui;
	else
		m_pUI = &m_ui2;

	//载入热键设置
	theApp.m_hot_key_enable = ini.GetBool(L"hot_key", L"hot_key_enable", true);

	SHotKey hot_key;
	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"play_pause", L"Ctrl+Shift+116"));
	theApp.m_hot_key.SetHotKey(HK_PLAY_PAUSE, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"stop", L"Ctrl+Shift+117"));
	theApp.m_hot_key.SetHotKey(HK_STOP, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"fast_forward", L"Ctrl+Shift+119"));
	theApp.m_hot_key.SetHotKey(HK_FF, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"rewind", L"Ctrl+Shift+118"));
	theApp.m_hot_key.SetHotKey(HK_REW, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"previous", L"Ctrl+Shift+37"));
	theApp.m_hot_key.SetHotKey(HK_PREVIOUS, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"next", L"Ctrl+Shift+39"));
	theApp.m_hot_key.SetHotKey(HK_NEXT, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"volume_up", L"Ctrl+Shift+38"));
	theApp.m_hot_key.SetHotKey(HK_VOLUME_UP, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"volume_down", L"Ctrl+Shift+40"));
	theApp.m_hot_key.SetHotKey(HK_VOLUME_DOWN, hot_key);

	hot_key = CHotkeyManager::HotkeyFromString(ini.GetString(L"hot_key", L"exit", L""));
	theApp.m_hot_key.SetHotKey(HK_EXIT, hot_key);

}

void CMusicPlayerDlg::SetTransparency()
{
	SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, theApp.m_app_setting_data.window_transparency * 255 / 100, LWA_ALPHA);  //透明度取值范围为0~255
}

void CMusicPlayerDlg::DrawInfo(bool reset)
{
	if (!IsIconic() && IsWindowVisible())		//窗口最小化或隐藏时不绘制，以降低CPU利用率
		m_pUI->DrawInfo(reset);
}

//void CMusicPlayerDlg::DrawLyricsSingleLine(CRect lyric_rect)
//{
//}
//
//void CMusicPlayerDlg::DrawLyricsMulityLine(CRect lyric_rect, CDC* pDC)
//{
//}

void CMusicPlayerDlg::SetPlaylistSize(int cx, int cy)
{
	//设置播放列表大小
	int width0, width1, width2;
	if (!m_ui_data.m_narrow_mode)
	{
		m_playlist_list.MoveWindow(cx / 2 + m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->search_edit_height + m_pLayout->path_edit_height + m_pLayout->margin,
			cx / 2 - 2 * m_pLayout->margin, cy - m_pLayout->control_bar_height - m_pLayout->search_edit_height - m_pLayout->path_edit_height - 2 * m_pLayout->margin);
		width0 = theApp.DPI(40);
		width2 = theApp.DPI(50);
		width1 = cx / 2 - width0 - width2 - theApp.DPI(21) - 2 * m_pLayout->margin;
	}
	else
	{
		m_playlist_list.MoveWindow(m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->info_height + m_pLayout->progress_bar_height + m_pLayout->search_edit_height + m_pLayout->path_edit_height,
			cx - 2 * m_pLayout->margin, cy - m_pLayout->control_bar_height - m_pLayout->info_height - m_pLayout->progress_bar_height - m_pLayout->search_edit_height - m_pLayout->path_edit_height - m_pLayout->margin);
		width0 = theApp.DPI(40);
		width2 = theApp.DPI(50);
		width1 = cx - width0 - width2 - theApp.DPI(21) - 2 * m_pLayout->margin;
	}
	m_playlist_list.SetColumnWidth(0, width0);
	m_playlist_list.SetColumnWidth(1, width1);
	m_playlist_list.SetColumnWidth(2, width2);
	//设置“当前路径”static控件大小
	CRect rect_static;
	m_path_static.GetWindowRect(rect_static);
	rect_static.bottom = rect_static.top + m_pLayout->path_edit_height - 2 * m_pLayout->margin;
	if (!m_ui_data.m_narrow_mode)
		rect_static.MoveToXY(cx / 2 + m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->margin);
	else
		rect_static.MoveToXY(m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->info_height + m_pLayout->progress_bar_height);
	m_path_static.MoveWindow(rect_static);
	//设置“当前路径”edit控件大小
	CRect rect_edit;
	m_path_edit.GetWindowRect(rect_edit);
	if (!m_ui_data.m_narrow_mode)
	{
		rect_edit.right = rect_edit.left + (cx / 2 - 2 * m_pLayout->margin - rect_static.Width());
		rect_edit.MoveToXY(cx / 2 + m_pLayout->margin + rect_static.Width(), m_pLayout->control_bar_height + m_pLayout->margin);
	}
	else
	{
		rect_edit.right = rect_edit.left + (cx - 2 * m_pLayout->margin - rect_static.Width());
		rect_edit.MoveToXY(m_pLayout->margin + rect_static.Width(), m_pLayout->control_bar_height + m_pLayout->info_height + m_pLayout->progress_bar_height);
	}
	m_path_edit.MoveWindow(rect_edit);
	//设置歌曲搜索框的大小和位置
	CRect rect_search;
	m_search_edit.GetWindowRect(rect_search);
	if (!m_ui_data.m_narrow_mode)
	{
		rect_search.right = rect_search.left + (cx / 2 - 2 * m_pLayout->margin - m_pLayout->margin - rect_search.Height());
		rect_search.MoveToXY(cx / 2 + m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->path_edit_height + theApp.DPI(1));
	}
	else
	{
		rect_search.right = rect_search.left + (cx - 2 * m_pLayout->margin - m_pLayout->margin - rect_search.Height());
		rect_search.MoveToXY(m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->info_height + m_pLayout->progress_bar_height + m_pLayout->path_edit_height - theApp.DPI(3));
	}
	m_search_edit.MoveWindow(rect_search);
	//设置清除搜索按钮的大小和位置
	CRect rect_clear{};
	rect_clear.right = rect_clear.bottom = rect_search.Height();
	//if (!m_ui_data.m_narrow_mode)
		rect_clear.MoveToXY(rect_search.right + m_pLayout->margin, rect_search.top);
	m_clear_search_button.MoveWindow(rect_clear);
	m_clear_search_button.Invalidate();
}

void CMusicPlayerDlg::SetPorgressBarSize(int cx, int cy)
{
	CRect rect;
	m_progress_bar.GetWindowRect(rect);
	int left_pos;		//进度条左侧位置
	int progress_width;	//进度条宽度
	int time_width;	//时间的宽度
	if (theApp.m_player.GetSongLength() >= 6000000)	//如果歌曲长度大于6000000毫秒，即大于100分钟，则显示时间控件的宽度增加50%
		time_width = m_time_width * 3 / 2;
	else if (theApp.m_player.GetSongLength() >= 600000)	//如果歌曲长度大于600000毫秒，即大于10分钟，则显示时间控件的宽度增加25%
		time_width = m_time_width * 5 / 4;
	else
		time_width = m_time_width;
	if (!m_ui_data.m_narrow_mode)
	{
		//设置进度条位置
		left_pos = m_progress_bar_left_pos;
		progress_width = cx - left_pos - 2 * m_pLayout->margin - time_width;
		rect.right = rect.left + progress_width;
		rect.MoveToXY(left_pos, theApp.DPI(6));
		m_progress_bar.MoveWindow(rect);
		//设置时间位置
		m_time_static.SetWindowPos(NULL, cx - time_width - m_pLayout->margin, theApp.DPI(6), time_width, m_time_height, SWP_NOZORDER);
	}
	else
	{
		//设置进度条位置
		left_pos = m_pLayout->margin;
		progress_width = cx - 3 * m_pLayout->margin - time_width;
		rect.right = rect.left + progress_width;
		rect.MoveToXY(left_pos, theApp.DPI(2) + m_pLayout->control_bar_height);
		m_progress_bar.MoveWindow(rect);
		//设置时间位置
		m_time_static.SetWindowPos(NULL, cx - time_width - m_pLayout->margin, theApp.DPI(2) + m_pLayout->control_bar_height, time_width, m_time_height, SWP_NOZORDER);
	}
	m_time_static.Invalidate();
}

void CMusicPlayerDlg::SetPorgressBarSize()
{
	CRect rect;
	GetClientRect(rect);
	SetPorgressBarSize(rect.Width(), rect.Height());
}

void CMusicPlayerDlg::ShowPlayList()
{
	m_playlist_list.ShowPlaylist(m_display_format);
	m_playlist_list.SetCurSel(-1);
	//设置播放列表中突出显示的项目
	SetPlayListColor();
	//显示当前路径
	m_path_edit.SetWindowTextW(theApp.m_player.GetCurrentDir().c_str());

	m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());

	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.ShowPlaylist();
		//m_miniModeDlg.m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());
	}
}

void CMusicPlayerDlg::SetPlayListColor()
{
	m_playlist_list.SetHightItem(theApp.m_player.GetIndex());
	//m_playlist_list.SetColor(theApp.m_app_setting_data.theme_color);
	m_playlist_list.Invalidate(FALSE);
	m_playlist_list.EnsureVisible(theApp.m_player.GetIndex(), FALSE);
}

void CMusicPlayerDlg::SwitchTrack()
{
	//当切换正在播放的歌曲时设置播放列表中突出显示的项目
	SetPlayListColor();
	if (m_miniModeDlg.m_hWnd != NULL)
	{
		m_miniModeDlg.SetPlayListColor();
		//m_miniModeDlg.RePaint();
		m_miniModeDlg.Invalidate(FALSE);
	}
	//切换歌曲时如果当前歌曲的时间没有显示，则显示出来
	CString song_length_str;
	int index{ theApp.m_player.GetIndex() };
	song_length_str = m_playlist_list.GetItemText(index, 2);
	if (song_length_str == _T("-:--") && !theApp.m_player.GetAllSongLength(index).isZero())
	{
		m_playlist_list.SetItemText(index, 2, theApp.m_player.GetAllSongLength(index).time2str().c_str());
	}
	ShowTime();
	m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());
	SetPorgressBarSize();		//调整进度条在窗口中的大小和位置
	//if (m_miniModeDlg.m_hWnd != NULL)
	//	m_miniModeDlg.m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());
	DrawInfo(true);
}

void CMusicPlayerDlg::ShowTime()
{
	m_time_static.SetWindowText(theApp.m_player.GetTimeString().c_str());
}

void CMusicPlayerDlg::UpdateProgress()
{
	int position, length;
	position = theApp.m_player.GetCurrentPosition();
	length = theApp.m_player.GetSongLength();
	if (length == 0) length = 1;	//防止除数为0
	__int64 pos;	//进度条的宽度
	pos = static_cast<__int64>(position) * 1000 / length;	//先转换成__int64类型，防止乘以1000之后溢出
	m_progress_bar.SetPos(static_cast<int>(pos));
	//if (m_miniModeDlg.m_hWnd != NULL)
	//	m_miniModeDlg.m_progress_bar.SetPos(static_cast<int>(pos));
}

void CMusicPlayerDlg::UpdateTaskBarProgress()
{
#ifndef COMPILE_IN_WIN_XP
	//根据播放状态设置任务栏状态和进度
	if (theApp.m_play_setting_data.show_taskbar_progress)
	{
		int position, length;
		position = theApp.m_player.GetCurrentPosition();
		length = theApp.m_player.GetSongLength();
		if (position == 0) position = 1;
		if (length == 0) length = 1;
		if (theApp.m_player.IsError())
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_ERROR);
		else if (theApp.m_player.IsPlaying())
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_INDETERMINATE);
		else
			m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_PAUSED);
		m_pTaskbar->SetProgressValue(this->GetSafeHwnd(), position, length);
	}
	else
	{
		m_pTaskbar->SetProgressState(this->GetSafeHwnd(), TBPF_NOPROGRESS);
	}
#endif
}

void CMusicPlayerDlg::UpdatePlayPauseButton()
{
	if (theApp.m_player.IsPlaying() && !theApp.m_player.IsError())
	{
#ifndef COMPILE_IN_WIN_XP
		//更新任务栏缩略图上“播放/暂停”的图标
		m_thumbButton[1].hIcon = m_hPauseIcon;
		wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PAUSE));
		//更新任务按钮上的播放状态图标
		if(theApp.m_play_setting_data.show_playstate_icon)
			m_pTaskbar->SetOverlayIcon(m_hWnd, m_hPlayIcon_s, L"");
		else
			m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
#endif
		//更新主界面上“播放/暂停”的图标
		m_play_pause_button.SetImage(m_hPauseIcon_s, FALSE);
		m_play_pause_button.SetWindowText(CCommon::LoadText(IDS_PAUSE2));
	}
	else
	{
#ifndef COMPILE_IN_WIN_XP
		//更新任务栏缩略图上“播放/暂停”的图标
		m_thumbButton[1].hIcon = m_hPlayIcon;
		wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
		//更新任务按钮上的播放状态图标
		if (theApp.m_play_setting_data.show_playstate_icon && theApp.m_player.GetPlayingState2() == 1)
			m_pTaskbar->SetOverlayIcon(m_hWnd, m_hPauseIcon_s, L"");
		else
			m_pTaskbar->SetOverlayIcon(m_hWnd, NULL, L"");
#endif
		//更新主界面上“播放/暂停”的图标
		m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
		m_play_pause_button.SetWindowText(CCommon::LoadText(IDS_PLAY2));
	}
#ifndef COMPILE_IN_WIN_XP
	m_pTaskbar->ThumbBarUpdateButtons(m_hWnd, 3, m_thumbButton);
#endif
	if (m_miniModeDlg.m_hWnd != NULL)
		m_miniModeDlg.UpdatePlayPauseButton();
}

void CMusicPlayerDlg::SetThumbnailClipArea()
{
#ifndef COMPILE_IN_WIN_XP
	if (m_pTaskbar != nullptr)
		m_pTaskbar->SetThumbnailClip(m_hWnd, m_pUI->GetThumbnailClipArea());
#endif
}

void CMusicPlayerDlg::EnablePlaylist(bool enable)
{
	m_playlist_list.EnableWindow(enable);
	m_search_edit.EnableWindow(enable);
	m_clear_search_button.EnableWindow(enable);
}


void CMusicPlayerDlg::CreateDesktopShortcut()
{
	//如果目录下没有recent_path和song_data文件，就判断为是第一次运行程序，提示用户是否创建桌面快捷方式
	if (!CCommon::FileExist(theApp.m_song_data_path) && !CCommon::FileExist(theApp.m_recent_path_dat_path))
	{
		wstring shortcut_path;

		if (MessageBox(CCommon::LoadText(IDS_CREATE_SHORTCUT_INFO), NULL, MB_ICONQUESTION | MB_YESNO) == IDYES)
		{
			if (CCommon::CreateFileShortcut(theApp.m_desktop_path.c_str(), NULL, _T("MusicPlayer2.lnk")))
			{
				CString info;
				info = CCommon::LoadTextFormat(IDS_SHORTCUT_CREATED, { theApp.m_desktop_path });
				MessageBox(info, NULL, MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(CCommon::LoadText(IDS_SHORTCUT_CREAT_FAILED), NULL, MB_ICONWARNING);
			}
		}
	}
}

void CMusicPlayerDlg::ApplySettings(const COptionsDlg& optionDlg)
{
	//获取选项设置对话框中的设置数据
	if (theApp.m_lyric_setting_data.show_lyric_in_cortana == true && optionDlg.m_tab1_dlg.m_data.show_lyric_in_cortana == false)	//如果在选项中关闭了“在Cortana搜索框中显示歌词”的选项，则重置Cortana搜索框的文本
		m_cortana_lyric.ResetCortanaText();

	bool reload_sf2{ theApp.m_general_setting_data.sf2_path != optionDlg.m_tab3_dlg.m_data.sf2_path };
	bool gauss_blur_changed{ theApp.m_app_setting_data.background_gauss_blur != optionDlg.m_tab2_dlg.m_data.background_gauss_blur
		|| theApp.m_app_setting_data.gauss_blur_radius != optionDlg.m_tab2_dlg.m_data.gauss_blur_radius
		|| theApp.m_app_setting_data.album_cover_as_background != optionDlg.m_tab2_dlg.m_data.album_cover_as_background };
	bool output_device_changed{ theApp.m_play_setting_data.output_device != optionDlg.m_tab4_dlg.m_data.output_device };

	theApp.m_lyric_setting_data = optionDlg.m_tab1_dlg.m_data;
	theApp.m_app_setting_data = optionDlg.m_tab2_dlg.m_data;
	theApp.m_general_setting_data = optionDlg.m_tab3_dlg.m_data;
	theApp.m_play_setting_data = optionDlg.m_tab4_dlg.m_data;
	theApp.m_hot_key.FromHotkeyGroup(optionDlg.m_tab5_dlg.m_hotkey_group);
	theApp.m_hot_key_enable = optionDlg.m_tab5_dlg.m_hot_key_enable;

	if (reload_sf2 || output_device_changed)		//如果在选项设置中更改了MIDI音频库的路径，则重新加载MIDI音频库
	{
		theApp.m_player.ReIniBASS(true);
		UpdatePlayPauseButton();
	}
	if (gauss_blur_changed)
		theApp.m_player.AlbumCoverGaussBlur();

	UpdatePlayPauseButton();

	CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);
	m_progress_bar.SetColor(theApp.m_app_setting_data.theme_color.original_color);		//设置进度条颜色
	m_progress_bar.Invalidate();
	m_time_static.Invalidate();
	SetPlayListColor();
	m_cortana_lyric.SetColors(theApp.m_app_setting_data.theme_color);
	m_cortana_lyric.SetCortanaColor(theApp.m_lyric_setting_data.cortana_color);
	ThemeColorChanged();
	if (optionDlg.m_tab2_dlg.FontChanged())
	{
		//如果m_font已经关联了一个字体资源对象，则释放它
		if (m_ui_data.lyric_font.m_hObject)
			m_ui_data.lyric_font.DeleteObject();
		m_ui_data.lyric_font.CreatePointFont(theApp.m_app_setting_data.lyric_font_size * 10, theApp.m_app_setting_data.lyric_font_name.c_str());
		if (m_ui_data.lyric_translate_font.m_hObject)
			m_ui_data.lyric_translate_font.DeleteObject();
		m_ui_data.lyric_translate_font.CreatePointFont((theApp.m_app_setting_data.lyric_font_size - 1) * 10, theApp.m_app_setting_data.lyric_font_name.c_str());
	}
	SaveConfig();		//将设置写入到ini文件
	theApp.SaveConfig();
	theApp.m_player.SaveConfig();
	DrawInfo(true);
}

void CMusicPlayerDlg::ThemeColorChanged()
{
	if (!theApp.m_app_setting_data.theme_color_follow_system)
		return;
	COLORREF color{};
	color = CCommon::GetWindowsThemeColor();
	//if (theApp.m_win_version.IsWindows10Version1809OrLater())		//Win10 1809版本的主题颜色过深，将其降低一点亮度
	CColorConvert::ReduceLuminance(color);
	if (theApp.m_app_setting_data.theme_color.original_color != color && color != RGB(255, 255, 255))	//当前主题色变了的时候重新设置主题色，但是确保获取到的颜色不是纯白色
	{
		theApp.m_app_setting_data.theme_color.original_color = color;
		m_progress_bar.SetColor(theApp.m_app_setting_data.theme_color.original_color);		//设置进度条颜色
		m_progress_bar.Invalidate();
		m_time_static.Invalidate();
		CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);
		SetPlayListColor();
		m_cortana_lyric.SetColors(theApp.m_app_setting_data.theme_color);
		DrawInfo();
		if (m_miniModeDlg.m_hWnd != NULL)
		{
			m_miniModeDlg.SetPlayListColor();
		}

	}
}

BOOL CMusicPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//// 将“关于...”菜单项添加到系统菜单中。

	//// IDM_ABOUTBOX 必须在系统命令范围内。
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_MINIMODE,CCommon::LoadText(IDS_MINI_MODE2, _T("\tCtrl+M")));
		pSysMenu->AppendMenu(MF_SEPARATOR);

		CCommon::AppendMenuOp(pSysMenu->GetSafeHmenu(), GetMenu()->GetSafeHmenu());		//将主菜单添加到系统菜单

		//BOOL bNameValid;
		//CString strAboutMenu;
		//bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		//if (!strAboutMenu.IsEmpty())
		//{
		//	pSysMenu->AppendMenu(MF_SEPARATOR);
		//	pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		//}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//获取当前系统DPI设置
	theApp.GetDPIFromWindow(this);

	//载入图标资源
	theApp.LoadIconResource();

	//载入设置
	LoadConfig();

	//只有win8以上的系统才能跟随系统主题色
#ifdef COMPILE_IN_WIN_XP
	theApp.m_app_setting_data.theme_color_follow_system = false;
//#else
//	if (!theApp.m_win_version.IsWindows8OrLater())
//		theApp.m_app_setting_data.theme_color_follow_system = false;
#endif

	//只有Win10以上的系统才能在Cortana搜索框中显示歌词
	if (!theApp.m_win_version.IsWindows10OrLater())
		theApp.m_lyric_setting_data.show_lyric_in_cortana = false;
	m_cortana_lyric.SetEnable(theApp.m_win_version.IsWindows10OrLater());

	//设置窗口不透明度
	SetTransparency();

	//初始化窗口大小
	//rect.right = m_window_width;
	//rect.bottom = m_window_height;
	if (m_window_height != -1 && m_window_width != -1)
	{
		//MoveWindow(rect);
		SetWindowPos(nullptr, 0, 0, m_window_width, m_window_height, SWP_NOZORDER | SWP_NOMOVE);
	}

	//根据当前系统的DPI设置窗口上方工具区和状态栏等的大小
	m_pLayout = std::make_shared<SLayoutData>();

	CRect rect1;
	m_time_static.GetWindowRect(rect1);
	m_time_width = rect1.Width();		//保存时间控件初始时的宽度
	m_time_height = rect1.Height();

	//初始化提示信息
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	m_Mytip.AddTool(GetDlgItem(ID_SET_PATH), CCommon::LoadText(IDS_SELECT_RENENT_FOLDER));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_DOWN), _T("减小音量"));
	//m_Mytip.AddTool(GetDlgItem(IDC_VOLUME_UP), _T("增大音量"));
	m_Mytip.AddTool(&m_time_static, CCommon::LoadText(IDS_PLAY_TIME));
	m_Mytip.AddTool(&m_clear_search_button, CCommon::LoadText(IDS_CLEAR_SEARCH_RESULT));
	m_Mytip.AddTool(&m_search_edit, CCommon::LoadText(IDS_INPUT_KEY_WORD));
	m_ui.SetToolTip(&m_Mytip);
	m_ui2.SetToolTip(&m_Mytip);

	//为显示播放时间的static控件设置SS_NOTIFY属性，以启用鼠标提示
	DWORD dwStyle = m_time_static.GetStyle();
	::SetWindowLong(m_time_static.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	m_list_popup_menu.LoadMenu(IDR_POPUP_MENU);		//装载播放列表右键菜单

	m_search_edit.SetCueBanner(CCommon::LoadText(IDS_SEARCH_HERE), TRUE);

	CoInitialize(0);	//初始化COM组件，用于支持任务栏显示进度和缩略图按钮
#ifndef COMPILE_IN_WIN_XP
	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pTaskbar));	//创建ITaskbarList3的实例

	//载入按钮图标
	m_hPlayPauseIcon = AfxGetApp()->LoadIcon(IDI_PLAY_PAUSE);
	m_hPreviousIcon = AfxGetApp()->LoadIcon(IDI_PREVIOUS);
	m_hNextIcon = AfxGetApp()->LoadIcon(IDI_NEXT1);
	m_hPlayIcon = AfxGetApp()->LoadIcon(IDI_PLAY);
	m_hPauseIcon = AfxGetApp()->LoadIcon(IDI_PAUSE);

	//初始化任务栏缩略图中的按钮
	THUMBBUTTONMASK dwMask = THB_ICON | THB_TOOLTIP | THB_FLAGS;
	//上一曲按钮
	m_thumbButton[0].dwMask = dwMask;
	m_thumbButton[0].iId = IDT_PREVIOUS;
	m_thumbButton[0].hIcon = m_hPreviousIcon;
	wcscpy_s(m_thumbButton[0].szTip, CCommon::LoadText(IDS_PREVIOUS));
	m_thumbButton[0].dwFlags = THBF_ENABLED;
	//播放/暂停按钮
	m_thumbButton[1].dwMask = dwMask;
	m_thumbButton[1].iId = IDT_PLAY_PAUSE;
	m_thumbButton[1].hIcon = m_hPlayIcon;
	wcscpy_s(m_thumbButton[1].szTip, CCommon::LoadText(IDS_PLAY));
	m_thumbButton[1].dwFlags = THBF_ENABLED;
	//下一曲按钮
	m_thumbButton[2].dwMask = dwMask;
	m_thumbButton[2].iId = IDT_NEXT;
	m_thumbButton[2].hIcon = m_hNextIcon;
	wcscpy_s(m_thumbButton[2].szTip, CCommon::LoadText(IDS_NEXT));
	m_thumbButton[2].dwFlags = THBF_ENABLED;
#endif

	//载入按钮小图标（16*16）
	m_hPreviousIcon_s = CDrawCommon::LoadIconResource(IDI_PREVIOUS, theApp.DPI(16), theApp.DPI(16));
	m_hNextIcon_s = CDrawCommon::LoadIconResource(IDI_NEXT1, theApp.DPI(16), theApp.DPI(16));
	m_hPlayIcon_s = CDrawCommon::LoadIconResource(IDI_PLAY, theApp.DPI(16), theApp.DPI(16));
	m_hPauseIcon_s = CDrawCommon::LoadIconResource(IDI_PAUSE, theApp.DPI(16), theApp.DPI(16));
	m_hStopIcon_s = CDrawCommon::LoadIconResource(IDI_STOP, theApp.DPI(16), theApp.DPI(16));

	//设置主界面上的按钮图标
	m_play_pause_button.SetFont(this->GetFont());
	m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
	m_play_pause_button.SetWindowText(CCommon::LoadText(IDS_PLAY2));

	m_previous_button.SetFont(this->GetFont());
	m_previous_button.SetImage(m_hPreviousIcon_s);

	m_next_button.SetFont(this->GetFont());
	m_next_button.SetImage(m_hNextIcon_s);

	m_stop_button.SetFont(this->GetFont());
	m_stop_button.SetImage(m_hStopIcon_s);

	CRect rect;
	//初始化进度条
	m_progress_bar.SetRange(1000);		//设置进度条的范围
	m_progress_bar.SetProgressBarHeight(theApp.DPI(5));	//设置进度条的高度
	m_progress_bar.SetColor(theApp.m_app_setting_data.theme_color.original_color);			//设置进度条的颜色
	m_progress_bar.GetWindowRect(rect);
	m_progress_bar_left_pos = rect.left;		//用控件起始的位置作为普通界面模式下进度条控件左侧的位置

	//注册全局热键	
	if(theApp.m_hot_key_enable)
		theApp.m_hot_key.RegisterAllHotKey();

	//设置界面的颜色
	CColorConvert::ConvertColor(theApp.m_app_setting_data.theme_color);

	//初始化查找对话框中的数据
	m_findDlg.LoadConfig();

	//获取Cortana歌词
	m_cortana_lyric.Init();
	m_cortana_lyric.SetColors(theApp.m_app_setting_data.theme_color);
	m_cortana_lyric.SetCortanaColor(theApp.m_lyric_setting_data.cortana_color);
	m_cortana_lyric.SetCortanaIconBeat(&theApp.m_lyric_setting_data.cortana_icon_beat);

	//初始化绘图的类
	m_pDC = GetDC();
	//m_draw.Create(m_pDC, this);
	m_ui.Init(m_pDC);
	m_ui2.Init(m_pDC);
	//m_pUI = &m_ui2;

	m_ui_data.lyric_font.CreatePointFont(theApp.m_app_setting_data.lyric_font_size * 10, theApp.m_app_setting_data.lyric_font_name.c_str());
	m_ui_data.lyric_translate_font.CreatePointFont((theApp.m_app_setting_data.lyric_font_size - 1) * 10, theApp.m_app_setting_data.lyric_font_name.c_str());		//歌词翻译字体比歌词字体小一号

	//载入默认背景图片（用于没有专辑封面时显示）
	m_ui_data.default_background.Load((theApp.m_local_dir + L"default_background.jpg").c_str());

	m_notify_icon.Init(m_hIcon);
	m_notify_icon.AddNotifyIcon();

	//设置定时器
	SetTimer(TIMER_ID, TIMER_ELAPSE, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMusicPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	DWORD cmd = nID & 0xFFF0;
	/*if (cmd == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else */if (cmd == IDM_MINIMODE)
	{
		OnMiniMode();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

	if (nID >= 0x8000)
	{
		SendMessage(WM_COMMAND, nID);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMusicPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	//DrawInfo();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMusicPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMusicPlayerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED && m_pLayout != nullptr)
	{
		if (m_pDC != NULL)
		{
			DrawInfo(true);
			if ((cx < m_pLayout->width_threshold) != m_ui_data.m_narrow_mode)	//如果在窄界面模式和普通模式之间进行了切换，则重绘客户区
			{
				Invalidate(FALSE);
				m_time_static.Invalidate(FALSE);
			}
			else
			{
				m_pUI->OnSizeRedraw(cx, cy);
			}
		}
		m_ui_data.client_width = cx;
		m_ui_data.client_height = cy;
		if (m_pLayout->width_threshold != 0)
		{
			m_ui_data.m_narrow_mode = (cx < m_pLayout->width_threshold);
		}
		if (m_playlist_list.m_hWnd && theApp.m_dpi)
		{
			SetPlaylistSize(cx, cy);
		}
		if (m_progress_bar.m_hWnd && theApp.m_dpi)
		{
			SetPorgressBarSize(cx, cy);
		}

		if (nType != SIZE_MAXIMIZED)
		{
			CRect rect;
			GetWindowRect(&rect);
			m_window_width = rect.Width();
			m_window_height = rect.Height();
		}

		//窗口大小变化时更新界面鼠标提示的位置
		static UINT last_type;
		if (last_type!=nType)
		{
			if (m_pUI != nullptr)
				DrawInfo();
		}
		last_type = nType;
		m_pUI->UpdateToolTipPosition();
	}

	SetThumbnailClipArea();


	// TODO: 在此处添加消息处理程序代码
}


void CMusicPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_first_start)
	{
		//此if语句只在定时器第1次触发时才执行
		m_first_start = false;
#ifndef COMPILE_IN_WIN_XP
		//设置任务栏缩略图窗口按钮
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
#endif
		CRect rect;
		GetClientRect(rect);
		m_ui_data.client_width = rect.Width();
		m_ui_data.client_height = rect.Height();
		SetPlaylistSize(rect.Width(), rect.Height());		//调整播放列表的大小和位置
		m_path_static.Invalidate();
		SetPorgressBarSize(rect.Width(), rect.Height());		//调整进度条在窗口中的大小和位置

		if (m_cmdLine.empty())		//没有有通过命令行打开文件
		{
			theApp.m_player.Create();
		}
		//else if (m_cmdLine.find(L"RestartByRestartManager") != wstring::npos)		//如果命令行参数中有RestartByRestartManager，则说明程序是被Windows重启的
		//{
		//	theApp.m_player.Create();
		//	//将命令行参数写入日志文件
		//	wchar_t buff[256];
		//	swprintf_s(buff, L"程序已被Windows的RestartManager重启，重启参数：%s", m_cmdLine.c_str());
		//	CCommon::WriteLog((CCommon::GetExePath() + L"error.log").c_str(), wstring{ buff });
		//}
		else		//从命令行参数获取要打开的文件
		{
			vector<wstring> files;
			wstring path = CCommon::DisposeCmdLine(m_cmdLine, files);
			if (!path.empty())
				theApp.m_player.Create(path);
			else
				theApp.m_player.Create(files);
			//MessageBox(m_cmdLine.c_str(), NULL, MB_ICONINFORMATION);
		}
		DrawInfo();
		SetThumbnailClipArea();

		//注：不应该在这里打开或播放歌曲，应该在播放列表初始化完毕时执行。
		//theApp.m_player.MusicControl(Command::OPEN);
		//theApp.m_player.MusicControl(Command::SEEK);
		//theApp.m_player.GetBASSError();
		SetPorgressBarSize(rect.Width(), rect.Height());		//重新调整进度条在窗口中的大小和位置（需要根据歌曲的时长调整显示时间控件的宽度）
		ShowTime();
		m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());

		//if(!m_cmdLine.empty())
		//	theApp.m_player.MusicControl(Command::PLAY);	//如果文件是通过命令行打开的，则打开后直接播放

		UpdatePlayPauseButton();
		//SetForegroundWindow();
		//ShowPlayList();

		ThemeColorChanged();

		//提示用户是否创建桌面快捷方式
		CreateDesktopShortcut();
	}

	m_timer_count++;

	UpdateTaskBarProgress();
	UpdateProgress();

	theApp.m_player.GetBASSError();
	if (m_miniModeDlg.m_hWnd == NULL && (theApp.m_player.IsPlaying() || GetActiveWindow() == this))		//进入迷你模式时不刷新，不在播放且窗口处于后台时不刷新
		DrawInfo();			//绘制界面上的信息（如果显示了迷你模式，则不绘制界面信息）
	theApp.m_player.GetBASSSpectral();
	if (theApp.m_player.IsPlaying())
	{
		theApp.m_player.GetBASSCurrentPosition();

		static int sec_temp;
		int sec_current = theApp.m_player.GetCurrentSecond();
		if (sec_temp != sec_current)		//获取播放时间的秒数，如果秒数变了则刷新一次时间
		{
			sec_temp = sec_current;
			ShowTime();
		}

		//在Cortana搜索框里显示歌词
		if (theApp.m_lyric_setting_data.show_lyric_in_cortana)
		{

			if (theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !theApp.m_player.MidiNoLyric())
			{
				wstring current_lyric{ theApp.m_player.GetMidiLyric() };
					m_cortana_lyric.DrawCortanaTextSimple(current_lyric.c_str(), Alignment::LEFT);
			}
			else if (!theApp.m_player.m_Lyrics.IsEmpty())		//有歌词时显示歌词
			{
				Time time{ theApp.m_player.GetCurrentPosition() };
				CLyrics::Lyric lyric = theApp.m_player.m_Lyrics.GetLyric(time, 0);
				int progress = theApp.m_player.m_Lyrics.GetLyricProgress(time);
				if (theApp.m_player.m_Lyrics.IsTranslated() && m_ui_data.show_translate)
				{
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					if (lyric.translate.empty()) lyric.translate = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					m_cortana_lyric.DrawLyricWithTranslate(lyric.text.c_str(), lyric.translate.c_str(), progress);
				}
				else if (!theApp.m_lyric_setting_data.cortana_lyric_double_line)
				{
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
					m_cortana_lyric.DrawCortanaText(lyric.text.c_str(), progress);
				}
				else
				{
					wstring next_lyric = theApp.m_player.m_Lyrics.GetLyric(time, 1).text;
					if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
					if (next_lyric.empty()) next_lyric = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
					m_cortana_lyric.DrawLyricDoubleLine(lyric.text.c_str(), next_lyric.c_str(), progress);
				}
			}
			else
			{
				//没有歌词时在Cortana搜索框上以滚动的方式显示当前播放歌曲的文件名
				static int index{};
				static wstring song_name{};
				//如果当前播放的歌曲发生变化，DrawCortanaText函数的第2参数为true，即重置滚动位置
				if (index != theApp.m_player.GetIndex() || song_name != theApp.m_player.GetFileName())
				{
					m_cortana_lyric.DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(theApp.m_player.GetCurrentSongInfo(), m_display_format).c_str()), true, theApp.DPI(2));
					index = theApp.m_player.GetIndex();
					song_name = theApp.m_player.GetFileName();
				}
				else
				{
					m_cortana_lyric.DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(theApp.m_player.GetCurrentSongInfo(), m_display_format).c_str()), false, theApp.DPI(2));
				}
			}
			//}

			//计算频谱，根据频谱幅值使Cortana图标显示动态效果
			float spectrum_avr{};		//取前面N个频段频谱值的平均值
			const int N = 8;
			for (int i{}; i <N; i++)
				spectrum_avr += theApp.m_player.GetFFTData()[i];
			spectrum_avr /= N;
			int spetraum = static_cast<int>(spectrum_avr * 4000);		//调整乘号后面的数值可以调整Cortana图标跳动时缩放的大小
			m_cortana_lyric.SetSpectrum(spetraum);
			//显示专辑封面，如果没有专辑封面，则显示Cortana图标
			m_cortana_lyric.AlbumCoverEnable(theApp.m_lyric_setting_data.cortana_show_album_cover/* && theApp.m_player.AlbumCoverExist()*/);
			m_cortana_lyric.DrawAlbumCover(theApp.m_player.GetAlbumCover());
		}
	}
	//if (theApp.m_player.SongIsOver() && (!theApp.m_lyric_setting_data.stop_when_error || !theApp.m_player.IsError()))	//当前曲目播放完毕且没有出现错误时才播放下一曲
	if ((theApp.m_player.SongIsOver() || (!theApp.m_play_setting_data.stop_when_error && theApp.m_player.IsError())) && m_play_error_cnt <= theApp.m_player.GetSongNum())	//当前曲目播放完毕且没有出现错误时才播放下一曲
	{
		if (theApp.m_player.IsError())
			m_play_error_cnt++;
		else
			m_play_error_cnt = 0;
		//当前正在编辑歌词，或顺序播放模式下列表中的歌曲播放完毕时（PlayTrack函数会返回false），播放完当前歌曲就停止播放
		if ((m_pLyricEdit != nullptr && m_pLyricEdit->m_dlg_exist) || !theApp.m_player.PlayTrack(NEXT))
		{
			theApp.m_player.MusicControl(Command::STOP);		//停止播放
			ShowTime();
			if (theApp.m_lyric_setting_data.show_lyric_in_cortana)
				m_cortana_lyric.ResetCortanaText();
		}
		SwitchTrack();
		UpdatePlayPauseButton();
	}
	if (theApp.m_player.IsPlaying() && (theApp.m_play_setting_data.stop_when_error && theApp.m_player.IsError()))
	{
		theApp.m_player.MusicControl(Command::PAUSE);
		UpdatePlayPauseButton();
	}

	if (m_timer_count % 10 == 0 && !m_cortana_lyric.m_cortana_disabled)
		m_cortana_lyric.CheckDarkMode();

	CDialog::OnTimer(nIDEvent);
}


void CMusicPlayerDlg::OnPlayPause()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.MusicControl(Command::PLAY_PAUSE);
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.MusicControl(Command::STOP);
	UpdatePlayPauseButton();
	ShowTime();
}


void CMusicPlayerDlg::OnPrevious()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.PlayTrack(PREVIOUS);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnNext()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.PlayTrack(NEXT);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnRew()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.MusicControl(Command::REW);
	UpdateTaskBarProgress();
	ShowTime();
}


void CMusicPlayerDlg::OnFF()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.MusicControl(Command::FF);
	UpdateTaskBarProgress();
	ShowTime();
}


void CMusicPlayerDlg::OnSetPath()
{
	// TODO: 在此添加命令处理程序代码
	CCommon::DeleteModelessDialog(m_pSetPathDlg);
	m_pSetPathDlg = new CSetPathDlg(theApp.m_player.GetRecentPath(), theApp.m_player.GetCurrentDir());
	m_pSetPathDlg->Create(IDD_SET_PATH_DIALOG);
	m_pSetPathDlg->ShowWindow(SW_SHOW);
}


afx_msg LRESULT CMusicPlayerDlg::OnPathSelected(WPARAM wParam, LPARAM lParam)
{
	if (m_pSetPathDlg != nullptr)
	{
		theApp.m_player.SetPath(m_pSetPathDlg->GetSelPath(), m_pSetPathDlg->GetTrack(), m_pSetPathDlg->GetPosition(), m_pSetPathDlg->GetSortMode());
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		ShowTime();
		DrawInfo(true);
		m_findDlg.ClearFindResult();		//更换路径后清除查找结果
		theApp.m_player.SaveRecentPath();
		m_play_error_cnt = 0;
	}
	return 0;
}


void CMusicPlayerDlg::OnFind()
{
	// TODO: 在此添加命令处理程序代码
	if (m_findDlg.DoModal() == IDOK)
	{
		if (m_findDlg.GetFindCurrentPlaylist())
		{
			int selected_track{ m_findDlg.GetSelectedTrack() };
			if (selected_track >= 0)
			{
				theApp.m_player.PlayTrack(m_findDlg.GetSelectedTrack());
				SwitchTrack();
				UpdatePlayPauseButton();
			}
		}
		else
		{
			wstring selected_song_path{ m_findDlg.GetSelectedSongPath() };
			if (!CCommon::FileExist(selected_song_path))
			{
				//如果文件不存在，则弹出错误信息
				CString info;
				info = CCommon::LoadTextFormat(IDS_CONNOT_FIND_FILE, { selected_song_path });
				MessageBox(info, NULL, MB_ICONWARNING);
				return;
			}
			if (!selected_song_path.empty())
			{
				theApp.m_player.OpenAFile(selected_song_path);
				SwitchTrack();
				UpdatePlayPauseButton();
			}
		}
	}
}


void CMusicPlayerDlg::OnExplorePath()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.ExplorePath();
}


BOOL CMusicPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd != m_search_edit.GetSafeHwnd())
	{
		//按下Ctrl键时
		if (GetKeyState(VK_CONTROL) & 0x80)
		{
			if (pMsg->wParam == 'O')		//设置按Ctr+O打开文件
			{
				OnFileOpen();
				return TRUE;
			}
			if (pMsg->wParam == 'D')		//设置按Ctr+D打开文件夹
			{
				OnFileOpenFolder();
				return TRUE;
			}
			if (pMsg->wParam == 'B')		//设置按Ctr+B浏览文件
			{
				OnExplorePath();
				return TRUE;
			}
			if (pMsg->wParam == 'N')		//设置按Ctr+N打开曲目信息
			{
				OnSongInfo();
				return TRUE;
			}
			if (pMsg->wParam == 'F')		//按Ctr+F键查找文件
			{
				OnFind();
				return TRUE;
			}
			if (pMsg->wParam == 'T')		//按Ctr+T键设置路径
			{
				OnSetPath();
				return TRUE;
			}
			if (pMsg->wParam == 'I')		//设置按Ctr+I打开设置
			{
				OnOptionSettings();
				return TRUE;
			}
			//if (pMsg->wParam == 'X')		//设置按Ctr+X退出
			//{
			//	OnMenuExit();
			//	return TRUE;
			//}
			if (pMsg->wParam == 'R')		//设置按Ctr+R打开录音机
			{
				OnRecorder();
				return TRUE;
			}
			if (pMsg->wParam == 'M')		//设置按Ctr+M进入迷你模式
			{
				OnMiniMode();
				return TRUE;
			}
			if (pMsg->wParam == VK_PRIOR)		//设置按Ctr+PgUp歌词提前0.5秒
			{
				OnLyricForward();
				return TRUE;
			}
			if (pMsg->wParam == VK_NEXT)		//设置按Ctr+PgDn歌词延后0.5秒
			{
				OnLyricDelay();
				return TRUE;
			}
			if (pMsg->wParam == 'S')		//设置按Ctr+S保存歌词更改
			{
				if (!theApp.m_player.m_Lyrics.IsEmpty() && theApp.m_player.m_Lyrics.IsModified())
					OnSaveModifiedLyric();
				return TRUE;
			}
			if (pMsg->wParam == 'E')		//设置按Ctr+S打开均衡器
			{
				OnEqualizer();
				return TRUE;
			}
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				//按下Ctrl + Shift键时
				if (pMsg->wParam == 'C')		//设置按Ctr+S打开均衡器
				{
					OnFormatConvert1();
					return TRUE;
				}
			}
		}
		else
		{
			if (pMsg->wParam == VK_SPACE || pMsg->wParam == 'P'/* || pMsg->wParam == VK_MEDIA_PLAY_PAUSE*/)		//按空格键/P键播放/暂停
			{
				OnPlayPause();
				return TRUE;
			}
			if (pMsg->wParam == 'S')	//按S键停止
			{
				OnStop();
				return TRUE;
			}
			if (pMsg->wParam == 'V')	//按V键上一曲
			{
				OnPrevious();
				return TRUE;
			}
			if (pMsg->wParam == 'N')	//按N键下一曲
			{
				OnNext();
				return TRUE;
			}
			if (pMsg->wParam == VK_LEFT)	//按左方向键下快退5秒
			{
				OnRew();
				return TRUE;
			}
			if (pMsg->wParam == VK_RIGHT)	//按右方向键下快进5秒
			{
				OnFF();
				return TRUE;
			}
			if (pMsg->wParam == VK_UP)	//按上方向键下音量加
			{
				OnVolumeUp();
				return TRUE;
			}
			if (pMsg->wParam == VK_DOWN)	//按下方向键音量减
			{
				OnVolumeDown();
				return TRUE;
			}
			if (pMsg->wParam == 'M')	//按M键设置循环模式
			{
				theApp.m_player.SetRepeatMode();
				m_ui.UpdateRepeatModeToolTip();
				m_ui2.UpdateRepeatModeToolTip();
				return TRUE;
			}
			if (pMsg->wParam == 'F')	//按F键快速查找
			{
				m_search_edit.SetFocus();
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))		//屏蔽按回车键和ESC键退出
	{
		return TRUE;
	}


	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);


	return CDialog::PreTranslateMessage(pMsg);
}


void CMusicPlayerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//退出时保存设置
	theApp.m_player.OnExit();
	SaveConfig();
	m_findDlg.SaveConfig();
	//解除全局热键
	theApp.m_hot_key.UnRegisterAllHotKey();

	//退出时恢复Cortana的默认文本
	m_cortana_lyric.ResetCortanaText();

	////退出时删除专辑封面临时文件
	//DeleteFile(theApp.m_player.GetAlbumCoverPath().c_str());

	m_notify_icon.DeleteNotifyIcon();
}


void CMusicPlayerDlg::OnAppAbout()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMusicPlayerDlg::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	vector<wstring> files;	//储存打开的多个文件路径
	//设置过滤器
	wstring filter(CCommon::LoadText(IDS_ALL_SUPPORTED_FORMAT, _T("|")));
	for (const auto& ext : CAudioCommon::m_all_surpported_extensions)
	{
		filter += L"*.";
		filter += ext;
		filter.push_back(L';');
	}
	filter.pop_back();
	filter.push_back(L'|');
	for (const auto& format : CAudioCommon::m_surpported_format)
	{
		filter += format.description;
		filter.push_back(L'|');
		filter += format.extensions_list;
		filter.push_back(L'|');
	}
	filter += CCommon::LoadText(IDS_ALL_FILES, _T("|*.*||"));
	//构造打开文件对话框
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_ALLOWMULTISELECT, filter.c_str(), this);
	//设置保存文件名的字符缓冲的大小为128kB（如果以平均一个文件名长度为32字节计算，最多可以打开大约4096个文件）
	fileDlg.m_ofn.nMaxFile = 128 * 1024;
	LPTSTR ch = new TCHAR[fileDlg.m_ofn.nMaxFile];
	fileDlg.m_ofn.lpstrFile = ch;
	//对内存块清零
	ZeroMemory(fileDlg.m_ofn.lpstrFile, sizeof(TCHAR) * fileDlg.m_ofn.nMaxFile);
	//显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		POSITION posFile = fileDlg.GetStartPosition();
		while (posFile != NULL)
		{
			files.push_back(fileDlg.GetNextPathName(posFile).GetString());
		}
		theApp.m_player.OpenFiles(files);
		//ShowPlayList();
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		DrawInfo(true);
		m_play_error_cnt = 0;
	}
	delete[] ch;
}


void CMusicPlayerDlg::OnFileOpenFolder()
{
	// TODO: 在此添加命令处理程序代码

#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(CCommon::LoadText(IDS_OPEN_FOLDER_INFO));
#else
	CFilePathHelper current_path(theApp.m_player.GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		theApp.m_player.OpenFolder(wstring(folderPickerDlg.GetPathName()));
		//ShowPlayList();
		UpdatePlayPauseButton();
		SetPorgressBarSize();
		DrawInfo(true);
		m_play_error_cnt = 0;
	}
}


void CMusicPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	vector<wstring> files;	//储存拖放到窗口的多个文件路径
	TCHAR file_path[MAX_PATH];
	int drop_count = DragQueryFile(hDropInfo, -1, NULL, 0);		//取得被拖动文件的数目
	//获取第1个文件
	DragQueryFile(hDropInfo, 0, file_path, MAX_PATH);
	wstring file_path_wcs{ file_path };
	if (file_path_wcs.size() > 4 && file_path_wcs[file_path_wcs.size() - 4] != L'.' && file_path_wcs[file_path_wcs.size() - 5] != L'.')
	{
		//如果获取的第一个文件的倒数第3个和倒数第4个字符都不是“.”，说明这不是一个文件，而是一个文件夹
		file_path_wcs.push_back(L'\\');
		theApp.m_player.OpenFolder(file_path_wcs);
	}
	else
	{
		for (int i{}; i < drop_count; i++)
		{
			DragQueryFile(hDropInfo, i, file_path, MAX_PATH);	//获取第i个文件
			if (CAudioCommon::FileIsAudio(wstring(file_path)))
				files.push_back(file_path);
		}
		theApp.m_player.OpenFiles(files, false);
	}
	//ShowPlayList();
	UpdatePlayPauseButton();
	SetPorgressBarSize();
	DrawInfo(true);

	CDialog::OnDropFiles(hDropInfo);
}


void CMusicPlayerDlg::OnInitMenu(CMenu* pMenu)
{
	CDialog::OnInitMenu(pMenu);
	//设置循环模式菜单的单选标记
	RepeatMode repeat_mode{ theApp.m_player.GetRepeatMode() };
	switch (repeat_mode)
	{
	case RM_PLAY_ORDER: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_ORDER, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_PLAY_SHUFFLE: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_SHUFFLE, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_PLAYLIST: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_PLAYLIST, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_TRACK: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	default: break;
	}

	//弹出右键菜单时，如果没有选中播放列表中的项目，则禁用右键菜单中“播放”、“从列表中删除”、“属性”、“从磁盘删除”项目。
	if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EXPLORE_ONLINE, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_COPY_FILE_TO, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_MOVE_FILE_TO, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EXPLORE_ONLINE, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_COPY_FILE_TO, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_MOVE_FILE_TO, MF_BYCOMMAND | MF_ENABLED);
	}

	//打开菜单时，如果播放列表中没有歌曲，则禁用主菜单和右键菜单中的“打开文件位置”项目
	if (theApp.m_player.GetSongNum() == 0)
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_EXPLORE_PATH, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EXPLORE_TRACK, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_LYRIC_BATCH_DOWNLOAD, MF_BYCOMMAND | MF_ENABLED);
	}


	//设置播放列表菜单中排序方式的单选标记
	switch (theApp.m_player.m_sort_mode)
	{
	case SM_FILE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_FILE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TITLE: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ARTIST: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_ALBUM: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_ALBUM, MF_BYCOMMAND | MF_CHECKED); break;
	case SM_TRACK: pMenu->CheckMenuRadioItem(ID_SORT_BY_FILE, ID_SORT_BY_TRACK, ID_SORT_BY_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//设置播放列表菜单中“播放列表显示样式”的单选标记
	switch (m_display_format)
	{
	case DF_FILE_NAME: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_FILE_NAME, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_ARTIST_TITLE: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_ARTIST_TITLE, MF_BYCOMMAND | MF_CHECKED); break;
	case DF_TITLE_ARTIST: pMenu->CheckMenuRadioItem(ID_DISP_FILE_NAME, ID_DISP_TITLE_ARTIST, ID_DISP_TITLE_ARTIST, MF_BYCOMMAND | MF_CHECKED); break;
	}

	//设置播放列表右键菜单的默认菜单项
	pMenu->SetDefaultItem(ID_PLAY_ITEM);

	//根据歌词是否存在设置启用或禁用菜单项
	bool midi_lyric{ theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !theApp.m_player.MidiNoLyric() };
	bool lyric_disable{ midi_lyric || theApp.m_player.m_Lyrics.IsEmpty() };
	bool no_lyric{ theApp.m_player.m_Lyrics.IsEmpty() && theApp.m_player.MidiNoLyric() };
	//pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_CURRENT_LYRIC, MF_BYCOMMAND | (!no_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_COPY_ALL_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	//pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!theApp.m_player.m_Lyrics.IsEmpty() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_FORWARD, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_LYRIC_DELAY, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_SAVE_MODIFIED_LYRIC, MF_BYCOMMAND | ((!lyric_disable && theApp.m_player.m_Lyrics.IsModified()) ? MF_ENABLED : MF_GRAYED));
	if(midi_lyric)
		pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | MF_GRAYED);
	else
		pMenu->EnableMenuItem(ID_DELETE_LYRIC, MF_BYCOMMAND | (CCommon::FileExist(theApp.m_player.m_Lyrics.GetPathName()) ? MF_ENABLED : MF_GRAYED));		//当歌词文件存在时启用“删除歌词”菜单项
	pMenu->EnableMenuItem(ID_BROWSE_LYRIC, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_TRANSLATE_TO_SIMPLIFIED_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_TRANSLATE_TO_TRANDITIONAL_CHINESE, MF_BYCOMMAND | (!lyric_disable ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_RELOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_EDIT_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DOWNLOAD_LYRIC, MF_BYCOMMAND | (!midi_lyric ? MF_ENABLED : MF_GRAYED));

	pMenu->EnableMenuItem(ID_ALBUM_COVER_SAVE_AS, MF_BYCOMMAND | (theApp.m_player.AlbumCoverExist() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DOWNLOAD_ALBUM_COVER, MF_BYCOMMAND | (!theApp.m_player.IsInnerCover() ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_DELETE_ALBUM_COVER, MF_BYCOMMAND | ((!theApp.m_player.IsInnerCover() && theApp.m_player.AlbumCoverExist()) ? MF_ENABLED : MF_GRAYED));

	//正在执行格式转换时禁用“格式转换”菜单项
	pMenu->EnableMenuItem(ID_FORMAT_CONVERT, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));
	pMenu->EnableMenuItem(ID_FORMAT_CONVERT1, MF_BYCOMMAND | (theApp.m_format_convert_dialog_exit ? MF_ENABLED : MF_GRAYED));

	// TODO: 在此处添加消息处理程序代码
}


void CMusicPlayerDlg::OnPlayOrder()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.SetRepeatMode(RM_PLAY_ORDER);		//设置顺序播放
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnPlayShuffle()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.SetRepeatMode(RM_PLAY_SHUFFLE);		//设置随机播放
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopPlaylist()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.SetRepeatMode(RM_LOOP_PLAYLIST);		//设置列表循环
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


void CMusicPlayerDlg::OnLoopTrack()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.SetRepeatMode(RM_LOOP_TRACK);		//设置单曲循环
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
}


BOOL CMusicPlayerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0)
	{
		OnVolumeUp();
	}
	if (zDelta < 0)
	{
		OnVolumeDown();
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CMusicPlayerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//限制窗口最小大小
	lpMMI->ptMinTrackSize.x = theApp.DPI(360);		//设置最小宽度
	lpMMI->ptMinTrackSize.y = theApp.DPI(360);		//设置最小高度

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CMusicPlayerDlg::OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (!m_searched)	//如果播放列表不在搜索状态，则当前选中项的行号就是曲目的索引
	{
		if (pNMItemActivate->iItem < 0)
			return;
		theApp.m_player.PlayTrack(pNMItemActivate->iItem);
	}
	else		//如果播放列表处理选中状态，则曲目的索引是选中行第一列的数字-1
	{
		int song_index;
		CString str;
		str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
		song_index = _ttoi(str) - 1;
		if (song_index < 0)
			return;
		theApp.m_player.PlayTrack(song_index);
	}
	SwitchTrack();
	UpdatePlayPauseButton();

	*pResult = 0;
}


//void CMusicPlayerDlg::OnRefreshPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	ShowPlayList();
//}


void CMusicPlayerDlg::OnOptionSettings()
{
	// TODO: 在此添加命令处理程序代码

	theApp.m_hot_key.UnRegisterAllHotKey();

	COptionsDlg optionDlg;
	//初始化对话框中变量的值
	optionDlg.m_tab_selected = m_tab_selected;
	optionDlg.m_tab1_dlg.m_data = theApp.m_lyric_setting_data;
	optionDlg.m_tab2_dlg.m_hMainWnd = m_hWnd;
	optionDlg.m_tab2_dlg.m_data = theApp.m_app_setting_data;
	optionDlg.m_tab3_dlg.m_data = theApp.m_general_setting_data;
	optionDlg.m_tab4_dlg.m_data = theApp.m_play_setting_data;
	optionDlg.m_tab5_dlg.m_hotkey_group = theApp.m_hot_key.GetHotKeyGroup();
	optionDlg.m_tab5_dlg.m_hot_key_enable = theApp.m_hot_key_enable;

	int sprctrum_height = theApp.m_app_setting_data.sprctrum_height;		//保存theApp.m_app_setting_data.sprctrum_height的值，如果用户点击了选项对话框的取消，则需要把恢复为原来的
	int background_transparency = theApp.m_app_setting_data.background_transparency;		//同上

	if (optionDlg.DoModal() == IDOK)
	{
		ApplySettings(optionDlg);
	}
	else
	{
		SetTransparency();		//如果点击了取消，则需要重新设置窗口透明度
		theApp.m_app_setting_data.sprctrum_height = sprctrum_height;
		theApp.m_app_setting_data.background_transparency = background_transparency;
	}

	m_tab_selected = optionDlg.m_tab_selected;
	
	if(theApp.m_hot_key_enable)
		theApp.m_hot_key.RegisterAllHotKey();

}


void CMusicPlayerDlg::OnReloadPlaylist()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.ReloadPlaylist();
	//ShowPlayList();
	//UpdatePlayPauseButton();
	//ShowTime();
}


void CMusicPlayerDlg::OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (!m_searched)
	{
		m_item_selected = pNMItemActivate->iItem;	//获取鼠标选中的项目
		m_playlist_list.GetItemSelected(m_items_selected);		//获取多个选中的项目
	}
	else
	{
		CString str;
		str = m_playlist_list.GetItemText(pNMItemActivate->iItem, 0);
		m_item_selected = _ttoi(str) - 1;
		m_playlist_list.GetItemSelectedSearched(m_items_selected);
	}

	CMenu* pContextMenu = m_list_popup_menu.GetSubMenu(0); //获取第一个弹出菜单
	CPoint point;			//定义一个用于确定光标位置的位置  
	GetCursorPos(&point);	//获取当前光标的位置，以便使得菜单可以跟随光标

	if (m_item_selected >= 0 && m_item_selected < theApp.m_player.GetSongNum())
	{
		CRect item_rect;
		m_playlist_list.GetItemRect(pNMItemActivate->iItem, item_rect, LVIR_BOUNDS);		//获取选中项目的矩形区域（以播放列表控件左上角为原点）
		CRect playlist_rect;
		m_playlist_list.GetWindowRect(playlist_rect);		//获取播放列表控件的矩形区域（以屏幕左上角为原点）
		point.y = playlist_rect.top + item_rect.bottom;	//设置鼠标要弹出的y坐标为选中项目的下边框位置，防止右键菜单挡住选中的项目
	}

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //在指定位置显示弹出菜单

	*pResult = 0;
}


void CMusicPlayerDlg::OnPlayItem()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.PlayTrack(m_item_selected);
	SwitchTrack();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnItemProperty()
{
	// TODO: 在此添加命令处理程序代码
	CPropertyDlg propertyDlg(theApp.m_player.GetPlayList());
	propertyDlg.m_path = theApp.m_player.GetCurrentDir();
	propertyDlg.m_index = m_item_selected;
	propertyDlg.m_song_num = theApp.m_player.GetSongNum();
	//propertyDlg.m_playing_index = theApp.m_player.GetIndex();
	//propertyDlg.m_lyric_name = theApp.m_player.GetLyricName();
	propertyDlg.DoModal();
	if (propertyDlg.GetListRefresh())
		ShowPlayList();
}


//void CMusicPlayerDlg::OnRemoveFromPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	theApp.m_player.RemoveSong(m_item_selected);
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


//void CMusicPlayerDlg::OnClearPlaylist()
//{
//	// TODO: 在此添加命令处理程序代码
//	theApp.m_player.ClearPlaylist();
//	ShowPlayList();
//	UpdatePlayPauseButton();
//}


void CMusicPlayerDlg::OnExploreTrack()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.ExplorePath(m_item_selected);
}


BOOL CMusicPlayerDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//响应任务栏缩略图按钮
	WORD command = LOWORD(wParam);
	switch(command)
	{
	case IDT_PLAY_PAUSE: OnPlayPause(); break;
	case IDT_PREVIOUS: OnPrevious(); break;
	case IDT_NEXT: OnNext(); break;
	}

	return CDialog::OnCommand(wParam, lParam);
}


void CMusicPlayerDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//响应全局快捷键
	switch (nHotKeyId)
	{
	case HK_PLAY_PAUSE: OnPlayPause(); break;
	case HK_PREVIOUS: OnPrevious(); break;
	case HK_NEXT: OnNext(); break;
	case HK_VOLUME_UP:
		OnVolumeUp();
		break;
	case HK_VOLUME_DOWN:
		OnVolumeDown();
		break;
	default: break;
	case HK_STOP:
		OnStop();
		break;
	case HK_FF:
		OnFF();
		break;
	case HK_REW:
		OnRew();
		break;
	case HK_EXIT:
		OnMenuExit();
		break;
	}

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}



void CMusicPlayerDlg::OnStnClickedProgressStatic()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_progress_bar.GetPos();
	int song_pos = static_cast<__int64>(pos) * theApp.m_player.GetSongLength() / 1000;
	theApp.m_player.SeekTo(song_pos);
	ShowTime();
}


void CMusicPlayerDlg::OnReIniBass()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.ReIniBASS();
	UpdatePlayPauseButton();
}


void CMusicPlayerDlg::OnSortByFile()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.m_sort_mode = SM_FILE;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTitle()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.m_sort_mode = SM_TITLE;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByArtist()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.m_sort_mode = SM_ARTIST;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByAlbum()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.m_sort_mode = SM_ALBUM;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnSortByTrack()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.m_sort_mode = SM_TRACK;
	theApp.m_player.SortPlaylist();
	ShowPlayList();
}


void CMusicPlayerDlg::OnDeleteFromDisk()
{
	// TODO: 在此添加命令处理程序代码
	if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
		return;
	int rtn;
	wstring delected_file;
	vector<wstring> delected_files;
	if (m_items_selected.size() > 1)
	{
		CString info;
		info = CCommon::LoadTextFormat(IDS_DELETE_FILE_INQUARY, { m_items_selected.size() });
		if (MessageBox(info, NULL, MB_ICONWARNING | MB_OKCANCEL) != IDOK)
			return;
		if(CCommon::IsItemInVector(m_items_selected, theApp.m_player.GetIndex()))	//如果选中的文件中有正在播放的文件，则删除前必须先关闭文件
			theApp.m_player.MusicControl(Command::CLOSE);
		for (const auto& index : m_items_selected)
			delected_files.push_back(theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[index].file_name);
		rtn = CCommon::DeleteFiles(m_hWnd, delected_files);
	}
	else
	{
		if (m_item_selected == theApp.m_player.GetIndex())	//如果删除的文件是正在播放的文件，则删除前必须先关闭文件
			theApp.m_player.MusicControl(Command::CLOSE);
		delected_file = theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[m_item_selected].file_name;
		rtn = CCommon::DeleteAFile(m_hWnd, delected_file);
	}
	if (rtn == 0)
	{
		//如果文件删除成功，同时从播放列表中移除
		if (m_items_selected.size() > 1)
			theApp.m_player.RemoveSongs(m_items_selected);
		else
			theApp.m_player.RemoveSong(m_item_selected);
		ShowPlayList();
		UpdatePlayPauseButton();
		DrawInfo(true);
		//文件删除后同时删除和文件同名的图片文件和歌词文件
		if (m_items_selected.size() > 1)
		{
			for (auto& file : delected_files)
			{
				CFilePathHelper file_path(file);
				file = file_path.ReplaceFileExtension(L"jpg").c_str();
			}
			CCommon::DeleteFiles(m_hWnd, delected_files);
			for (auto& file : delected_files)
			{
				CFilePathHelper file_path(file);
				file = file_path.ReplaceFileExtension(L"lrc").c_str();
			}
			CCommon::DeleteFiles(m_hWnd, delected_files);
		}
		else
		{
			CFilePathHelper file_path(delected_file);
			CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"jpg").c_str());
			CCommon::DeleteAFile(m_hWnd, file_path.ReplaceFileExtension(L"lrc").c_str());
		}
	}
	else if (rtn == 1223)	//如果在弹出的对话框中点击“取消”则返回值为1223
	{
		if (m_item_selected == theApp.m_player.GetIndex())		//如果删除的文件是正在播放的文件，又点击了“取消”，则重新打开当前文件
		{
			theApp.m_player.MusicControl(Command::OPEN);
			theApp.m_player.MusicControl(Command::SEEK);
			//theApp.m_player.Refresh();
			UpdatePlayPauseButton();
			DrawInfo(true);
		}
	}
	else
	{
		MessageBox(CCommon::LoadText(IDS_CONNOT_DELETE_FILE), NULL, MB_ICONWARNING);
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnTaskbarcreated(WPARAM wParam, LPARAM lParam)
{
#ifndef COMPILE_IN_WIN_XP
	//当资源管理器重启后重新添加任务栏缩略图窗口按钮
	m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);
	SetThumbnailClipArea();
	//重新更新任务栏上的播放状态角标
	UpdatePlayPauseButton();
#endif
	//资源管理器重启后Cortana的句柄会发生改变，此时要重新获取Cortana的句柄
	m_cortana_lyric.Init();

	m_notify_icon.AddNotifyIcon();		//重新添加通知区图标

	return 0;
}


void CMusicPlayerDlg::OnDispFileName()
{
	// TODO: 在此添加命令处理程序代码
	m_display_format = DF_FILE_NAME;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitle()
{
	// TODO: 在此添加命令处理程序代码
	m_display_format = DF_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispArtistTitle()
{
	// TODO: 在此添加命令处理程序代码
	m_display_format = DF_ARTIST_TITLE;
	ShowPlayList();
}


void CMusicPlayerDlg::OnDispTitleArtist()
{
	// TODO: 在此添加命令处理程序代码
	m_display_format = DF_TITLE_ARTIST;
	ShowPlayList();
}


void CMusicPlayerDlg::OnMiniMode()
{
	// TODO: 在此添加命令处理程序代码
	if (m_miniModeDlg.m_hWnd != NULL)
		return;

	m_miniModeDlg.SetDefaultBackGround(&m_ui_data.default_background);
	m_miniModeDlg.SetDisplayFormat(&m_display_format);
	ShowWindow(SW_HIDE);
	if (m_miniModeDlg.DoModal() == IDCANCEL)
	{
		//SendMessage(WM_COMMAND, ID_APP_EXIT);
		OnMenuExit();
	}
	else
	{
		ShowWindow(SW_SHOW);
#ifndef COMPILE_IN_WIN_XP
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//重新添加任务栏缩略图按钮
		SetThumbnailClipArea();		//重新设置任务栏缩略图
#endif
		SetForegroundWindow();
		SwitchTrack();
		UpdatePlayPauseButton();
	}
}


void CMusicPlayerDlg::OnBnClickedStop()
{
	// TODO: 在此添加控件通知处理程序代码
	SetFocus();
	OnStop();
}


void CMusicPlayerDlg::OnBnClickedPrevious()
{
	// TODO: 在此添加控件通知处理程序代码
	SetFocus();
	OnPrevious();
}


void CMusicPlayerDlg::OnBnClickedPlayPause()
{
	// TODO: 在此添加控件通知处理程序代码
	SetFocus();
	OnPlayPause();
}


void CMusicPlayerDlg::OnBnClickedNext()
{
	// TODO: 在此添加控件通知处理程序代码
	SetFocus();
	OnNext();
}


//void CMusicPlayerDlg::OnMove(int x, int y)
//{
//	CDialog::OnMove(x, y);
//
//	// TODO: 在此处添加消息处理程序代码
//	SetMaskWindowPos();
//}


void CMusicPlayerDlg::OnReloadLyric()
{
	// TODO: 在此添加命令处理程序代码
	CWaitCursor wait_cursor;
	theApp.m_player.SearchLyrics();
	theApp.m_player.IniLyrics();
}


void CMusicPlayerDlg::OnSongInfo()
{
	// TODO: 在此添加命令处理程序代码
	CPropertyDlg propertyDlg(theApp.m_player.GetPlayList());
	propertyDlg.m_path = theApp.m_player.GetCurrentDir();
	propertyDlg.m_index = theApp.m_player.GetIndex();
	propertyDlg.m_song_num = theApp.m_player.GetSongNum();
	propertyDlg.DoModal();
	if (propertyDlg.GetListRefresh())
		ShowPlayList();
}


void CMusicPlayerDlg::OnCopyCurrentLyric()
{
	// TODO: 在此添加命令处理程序代码
	bool midi_lyric{ theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !theApp.m_player.MidiNoLyric() };
	wstring lyric_str;
	if (midi_lyric)
	{
		lyric_str = theApp.m_player.GetMidiLyric();
	}
	else
	{
		CLyrics::Lyric lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };
		lyric_str = lyric.text;
		if (m_ui_data.show_translate && !lyric.translate.empty())
		{
			lyric_str += L"\r\n";
			lyric_str += lyric.translate;
		}
	}
	if (!CCommon::CopyStringToClipboard(lyric_str))
	//	MessageBox(_T("当前歌词已成功复制到剪贴板。"), NULL, MB_ICONINFORMATION);
	//else
		MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnCopyAllLyric()
{
	// TODO: 在此添加命令处理程序代码
	if (CCommon::CopyStringToClipboard(theApp.m_player.m_Lyrics.GetAllLyricText(m_ui_data.show_translate)))
		MessageBox(CCommon::LoadText(IDS_ALL_LRYIC_COPIED), NULL, MB_ICONINFORMATION);
	else
		MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CMusicPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	////双击鼠标左键进入迷你模式
	//if (!m_ui_data.repetemode_rect.PtInRect(point) && !m_ui_data.volume_btn.rect.PtInRect(point)
	//	&& !m_ui_data.volume_up_rect.PtInRect(point) && !m_ui_data.volume_down_rect.PtInRect(point)
	//	&& !m_ui_data.translate_btn.rect.PtInRect(point))
	//	OnMiniMode();
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CMusicPlayerDlg::OnLyricForward()
{
	// TODO: 在此添加命令处理程序代码
	//歌词提前0.5秒
	theApp.m_player.m_Lyrics.AdjustLyric(-500);

}


void CMusicPlayerDlg::OnLyricDelay()
{
	// TODO: 在此添加命令处理程序代码
	//歌词延后0.5秒
	theApp.m_player.m_Lyrics.AdjustLyric(500);
}


void CMusicPlayerDlg::OnSaveModifiedLyric()
{
	// TODO: 在此添加命令处理程序代码
	if (theApp.m_lyric_setting_data.save_lyric_in_offset && !theApp.m_player.m_Lyrics.IsChineseConverted())		//如果执行了中文繁简转换，则保存时不管选项设置如何都调用SaveLyric2()
		theApp.m_player.m_Lyrics.SaveLyric();
	else
		theApp.m_player.m_Lyrics.SaveLyric2();
}


void CMusicPlayerDlg::OnEditLyric()
{
	// TODO: 在此添加命令处理程序代码
	//ShellExecute(NULL, _T("open"), theApp.m_player.m_Lyrics.GetPathName().c_str(), NULL, NULL, SW_SHOWNORMAL);
	CCommon::DeleteModelessDialog(m_pLyricEdit);
	m_pLyricEdit = new CLyricEditDlg;
	m_pLyricEdit->Create(IDD_LYRIC_EDIT_DIALOG);
	m_pLyricEdit->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnDownloadLyric()
{
	// TODO: 在此添加命令处理程序代码
	CLyricDownloadDlg aDlg;
	aDlg.DoModal();
}


void CMusicPlayerDlg::OnLyricBatchDownload()
{
	// TODO: 在此添加命令处理程序代码
	//CLyricBatchDownloadDlg aDlg;
	//aDlg.DoModal();
	CCommon::DeleteModelessDialog(m_pLyricBatchDownDlg);
	m_pLyricBatchDownDlg = new CLyricBatchDownloadDlg;
	m_pLyricBatchDownDlg->Create(IDD_LYRIC_BATCH_DOWN_DIALOG);
	m_pLyricBatchDownDlg->ShowWindow(SW_SHOW);

}


void CMusicPlayerDlg::OnDeleteLyric()
{
	// TODO: 在此添加命令处理程序代码
	if (CCommon::FileExist(theApp.m_player.m_Lyrics.GetPathName()))
	{
		int rtn = CCommon::DeleteAFile(m_hWnd, theApp.m_player.m_Lyrics.GetPathName());		//删除歌词文件
		theApp.m_player.ClearLyric();		//清除歌词关联
	}
}


void CMusicPlayerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_pUI->RButtonUp(point);

	CDialog::OnRButtonUp(nFlags, point);
}


void CMusicPlayerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pUI->MouseMove(point);

	CDialog::OnMouseMove(nFlags, point);
}


void CMusicPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_pUI->LButtonUp(point);

	CDialog::OnLButtonUp(nFlags, point);
}


HBRUSH CMusicPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//设置播放时间控件的颜色
	if (pWnd == &m_time_static)
	{
		pDC->SetTextColor(theApp.m_app_setting_data.theme_color.dark2);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam)
{
	theApp.DoWaitCursor(0);
	ShowPlayList();
	ShowTime();
	DrawInfo(true);
	SetPorgressBarSize();
	UpdatePlayPauseButton();
	ShowTime();
	EnablePlaylist(true);
	theApp.DoWaitCursor(-1);

	return 0;
}


afx_msg LRESULT CMusicPlayerDlg::OnSetTitle(WPARAM wParam, LPARAM lParam)
{
	CString title;
	title = CPlayListCtrl::GetDisplayStr(theApp.m_player.GetCurrentSongInfo(), m_display_format).c_str();

	CString title_suffix;
	if (!title.IsEmpty())
		title_suffix += _T(" - ");
	#ifdef _DEBUG
		title_suffix += CCommon::LoadText(_T("MusicPlayer2 "), IDS_DEBUG_MODE);
	#else
		title_suffix += _T("MusicPlayer2");
	#endif

	SetWindowText(title + title_suffix);		//用当前正在播放的歌曲名作为窗口标题

	int title_length = 128 - title_suffix.GetLength() - 1;
	if(title.GetLength() > title_length)
		title = title.Left(title_length);
	m_notify_icon.SetIconToolTip(title + title_suffix);

	return 0;
}


void CMusicPlayerDlg::OnEqualizer()
{
	// TODO: 在此添加命令处理程序代码
	CCommon::DeleteModelessDialog(m_pSoundEffecDlg);
	m_pSoundEffecDlg = new CSoundEffectDlg;
	m_pSoundEffecDlg->Create(IDD_SOUND_EFFECT_DIALOG);
	m_pSoundEffecDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnExploreOnline()
{
	// TODO: 在此添加命令处理程序代码
	m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)m_item_selected);
}

UINT CMusicPlayerDlg::ViewOnlineThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	//此命令用于跳转到歌曲对应的网易云音乐的在线页面
	int item_selected = (int)lpParam;
	if (item_selected >= 0 && item_selected < theApp.m_player.GetSongNum())
	{
		//查找歌曲并获取最佳匹配项的歌曲ID
		const SongInfo& song{ theApp.m_player.GetPlayList()[item_selected] };
		if (song.song_id.empty())		//如果没有获取过ID，则获取一次ID
		{
			wstring song_id;
			song_id = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name).id;
			theApp.m_player.SetRelatedSongID(item_selected, song_id);
		}

		if (song.song_id.empty())
			return 0;
		//获取网易云音乐中该歌曲的在线接听网址
		wstring song_url{ L"http://music.163.com/#/song?id=" + song.song_id };

		//打开超链接
		ShellExecute(NULL, _T("open"), song_url.c_str(), NULL, NULL, SW_SHOW);
	}
	return 0;
}

UINT CMusicPlayerDlg::DownloadLyricAndCoverThreadFunc(LPVOID lpParam)
{
	CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
	//CMusicPlayerDlg* pThis = (CMusicPlayerDlg*)lpParam;
	const SongInfo& song{ theApp.m_player.GetCurrentSongInfo() };
	if (theApp.m_general_setting_data.auto_download_only_tag_full)		//设置了“仅当歌曲信息完整进才自动下载”时，如果歌曲标题和艺术家有一个为空，则不自动下载
	{
		if ((song.title.empty() || song.title == CCommon::LoadText(IDS_DEFAULT_TITLE).GetString()) || (song.artist.empty() || song.artist == CCommon::LoadText(IDS_DEFAULT_ARTIST).GetString()))
			return 0;
	}

	bool download_cover{ theApp.m_general_setting_data.auto_download_album_cover && !theApp.m_player.AlbumCoverExist() && !theApp.m_player.GetCurrentSongInfo().is_cue };
	bool midi_lyric{ theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric };
	bool download_lyric{ theApp.m_general_setting_data.auto_download_lyric && theApp.m_player.m_Lyrics.IsEmpty() && !midi_lyric };
	CInternetCommon::ItemInfo match_item;
	if (download_cover || download_lyric)
	{
		if (song.song_id.empty())		//如果没有获取过ID，则获取一次ID
		{
			//搜索歌曲并获取最佳匹配的项目
			match_item = CInternetCommon::SearchSongAndGetMatched(song.title, song.artist, song.album, song.file_name, false);
			theApp.m_player.SetRelatedSongID(match_item.id);
		}
		if (song.song_id.empty())
			return 0;
	}
	//自动下载专辑封面
	if (download_cover)
	{
		wstring cover_url = CCoverDownloadCommon::GetAlbumCoverURL(song.song_id);
		if (cover_url.empty())
		{
			return 0;
		}

		//获取要保存的专辑封面的文件路径
		CFilePathHelper cover_file_path;
		if (match_item.album == song.album)		//如果在线搜索结果的唱片集名称和歌曲的相同，则以“唱片集”为文件名保存
		{
			wstring album_name{ match_item.album };
			CCommon::FileNameNormalize(album_name);
			cover_file_path.SetFilePath(theApp.m_player.GetCurrentDir() + album_name);
		}
		else				//否则以歌曲文件名为文件名保存
		{
			cover_file_path.SetFilePath(theApp.m_player.GetCurrentDir() + song.file_name);
		}
		CFilePathHelper url_path(cover_url);
		cover_file_path.ReplaceFileExtension(url_path.GetFileExtension().c_str());

		//下面专辑封面
		URLDownloadToFile(0, cover_url.c_str(), cover_file_path.GetFilePath().c_str(), 0, NULL);

		//将下载的专辑封面改为隐藏属性
		SetFileAttributes(cover_file_path.GetFilePath().c_str(), FILE_ATTRIBUTE_HIDDEN);

		//重新从本地获取专辑封面
		theApp.m_player.SearchOutAlbumCover();
		::PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_ALBUM_COVER_DOWNLOAD_COMPLETE, 0, 0);
	}
	//自动下载歌词
	if (download_lyric)
	{
		//下载歌词
		wstring lyric_str;
		if (!CLyricDownloadCommon::DownloadLyric(song.song_id, lyric_str, true))
			return 0;
		if (!CLyricDownloadCommon::DisposeLryic(lyric_str))
			return 0;
		CLyricDownloadCommon::AddLyricTag(lyric_str, match_item.id, match_item.title, match_item.artist, match_item.album);
		//保存歌词
		CFilePathHelper lyric_path;
		wstring file_name;
		if (!song.is_cue)
			file_name = song.file_name;
		else
			file_name = song.artist + L" - " + song.title;
		//if (CCommon::FolderExist(theApp.m_lyric_setting_data.lyric_path))
		//{
		//	lyric_path.SetFilePath(theApp.m_lyric_setting_data.lyric_path + theApp.m_player.GetCurrentSongInfo().file_name);
		//}
		//else
		//{
		lyric_path.SetFilePath(theApp.m_player.GetCurrentDir() + file_name);
		//}
		lyric_path.ReplaceFileExtension(L"lrc");
		string _lyric_str = CCommon::UnicodeToStr(lyric_str, CodeType::UTF8);
		ofstream out_put{ lyric_path.GetFilePath(), std::ios::binary };
		out_put << _lyric_str;
		out_put.close();
		//处理歌词翻译
		CLyrics lyrics{ lyric_path.GetFilePath() };		//打开保存过的歌词
		lyrics.DeleteRedundantLyric();		//删除多余的歌词
		lyrics.CombineSameTimeLyric();		//将歌词翻译和原始歌词合并成一句
		lyrics.SaveLyric2();

		theApp.m_player.IniLyrics(lyric_path.GetFilePath());
	}
	return 0;
}



afx_msg LRESULT CMusicPlayerDlg::OnPlaylistIniStart(WPARAM wParam, LPARAM lParam)
{
	EnablePlaylist(false);
	theApp.DoWaitCursor(1);
	return 0;
}


void CMusicPlayerDlg::OnBrowseLyric()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.ExploreLyric();
}


void CMusicPlayerDlg::OnTranslateToSimplifiedChinese()
{
	// TODO: 在此添加命令处理程序代码
	CWaitCursor wait_cursor;
	theApp.m_player.m_Lyrics.ChineseConvertion(true);
}


void CMusicPlayerDlg::OnTranslateToTranditionalChinese()
{
	// TODO: 在此添加命令处理程序代码
	CWaitCursor wait_cursor;
	theApp.m_player.m_Lyrics.ChineseConvertion(false);
}


void CMusicPlayerDlg::OnAlbumCoverSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	//设置过滤器
	CString szFilter = CCommon::LoadText(IDS_ALL_FILES, _T("(*.*)|*.*||"));
	//设置另存为时的默认文件名
	CString file_name;
	CString extension;
	if (theApp.m_player.IsInnerCover())
	{
		switch (theApp.m_player.GetAlbumCoverType())
		{
		case 0:
			extension = _T("jpg");
			break;
		case 1:
			extension = _T("png");
			break;
		case 2:
			extension = _T("gif");
			break;
		default: return;
		}
	}
	else
	{
		CFilePathHelper cover_path(theApp.m_player.GetAlbumCoverPath());
		extension = cover_path.GetFileExtension().c_str();
	}
	file_name.Format(_T("AlbumCover - %s - %s.%s"), theApp.m_player.GetCurrentSongInfo().artist.c_str(), theApp.m_player.GetCurrentSongInfo().album.c_str(), extension);
	wstring file_name_wcs{ file_name };
	CCommon::FileNameNormalize(file_name_wcs);		//替换掉文件名中的无效字符
	//构造保存文件对话框
	CFileDialog fileDlg(FALSE, _T("txt"), file_name_wcs.c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	//显示保存文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		CString dest_file = fileDlg.GetPathName();
		::CopyFile(theApp.m_player.GetAlbumCoverPath().c_str(), dest_file, FALSE);
		SetFileAttributes(dest_file, FILE_ATTRIBUTE_NORMAL);		//取消文件的隐藏属性
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnConnotPlayWarning(WPARAM wParam, LPARAM lParam)
{
	if (theApp.m_nc_setting_data.no_sf2_warning)
	{
		if (MessageBox(CCommon::LoadText(IDS_NO_MIDI_SF2_WARNING), NULL, MB_ICONWARNING | MB_OKCANCEL) == IDCANCEL)
			theApp.m_nc_setting_data.no_sf2_warning = false;
	}
	return 0;
}


void CMusicPlayerDlg::OnEnChangeSearchEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	m_search_edit.GetWindowText(str);
	m_search_key_word = str;
	m_searched = (str.GetLength() != 0);
	m_playlist_list.QuickSearch(m_search_key_word);
	m_playlist_list.ShowPlaylist(m_display_format, m_searched);
}


void CMusicPlayerDlg::OnBnClickedClearSearchButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_searched)
	{
		//清除搜索结果
		m_searched = false;
		m_search_edit.SetWindowText(_T(""));
		m_playlist_list.ShowPlaylist(m_display_format, m_searched);
		m_playlist_list.EnsureVisible(theApp.m_player.GetIndex(), FALSE);		//清除搜索结果后确保正在播放曲目可见
	}
}


void CMusicPlayerDlg::OnDownloadAlbumCover()
{
	// TODO: 在此添加命令处理程序代码
	//_OnDownloadAlbumCover(true);
	CCoverDownloadDlg dlg;
	dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnMusicStreamOpened(WPARAM wParam, LPARAM lParam)
{
	//专辑封面高斯模糊处理（放到这里是为了避免此函数在工作线程中被调用，在工作线程中，拉伸图片的处理CDrawCommon::BitmapStretch有一定的概率出错，原因未知）
	theApp.m_player.AlbumCoverGaussBlur();
	//自动下载专辑封面
	m_pDownloadThread = AfxBeginThread(DownloadLyricAndCoverThreadFunc, this);
	return 0;
}


void CMusicPlayerDlg::OnCurrentExploreOnline()
{
	// TODO: 在此添加命令处理程序代码
	m_pThread = AfxBeginThread(ViewOnlineThreadFunc, (void*)theApp.m_player.GetIndex());
}


void CMusicPlayerDlg::OnDeleteAlbumCover()
{
	// TODO: 在此添加命令处理程序代码
	theApp.m_player.DeleteAlbumCover();
}


void CMusicPlayerDlg::OnCopyFileTo()
{
	// TODO: 在此添加命令处理程序代码
	LPCTSTR title{ CCommon::LoadText(IDS_SELECT_COPY_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(title);
#else
	CFilePathHelper current_path(theApp.m_player.GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
	folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
			return;
		if (m_items_selected.size() > 1)
		{
			vector<wstring> source_files;
			for (const auto& index : m_items_selected)
				source_files.push_back(theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[index].file_name);
			CCommon::CopyFiles(this->GetSafeHwnd(), source_files, wstring(folderPickerDlg.GetPathName()));
		}
		else
		{
			wstring source_file = theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[m_item_selected].file_name;
			CCommon::CopyAFile(this->GetSafeHwnd(), theApp.m_player.GetCurrentFilePath(), wstring(folderPickerDlg.GetPathName()));
		}
	}
}


void CMusicPlayerDlg::OnMoveFileTo()
{
	// TODO: 在此添加命令处理程序代码
	LPCTSTR title{ CCommon::LoadText(IDS_SELECT_MOVE_TARGET_FOLDER) };
#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(title);
#else
	CFilePathHelper current_path(theApp.m_player.GetCurrentDir());
	CFolderPickerDialog folderPickerDlg(current_path.GetParentDir().c_str());
	folderPickerDlg.m_ofn.lpstrTitle = title;
#endif
	if (folderPickerDlg.DoModal() == IDOK)
	{
		if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
			return;
		wstring source_file;
		vector<wstring> source_files;
		int rtn;
		if (m_items_selected.size() > 1)
		{
			if (CCommon::IsItemInVector(m_items_selected, theApp.m_player.GetIndex()))	//如果选中的文件中有正在播放的文件，则移动前必须先关闭文件
				theApp.m_player.MusicControl(Command::CLOSE);
			for (const auto& index : m_items_selected)
				source_files.push_back(theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[index].file_name);
			rtn = CCommon::MoveFiles(m_hWnd, source_files, wstring(folderPickerDlg.GetPathName()));
		}
		else
		{
			if (m_item_selected == theApp.m_player.GetIndex())	//如果移动的文件是正在播放的文件，则移动前必须先关闭文件
				theApp.m_player.MusicControl(Command::CLOSE);
			source_file = theApp.m_player.GetCurrentDir() + theApp.m_player.GetPlayList()[m_item_selected].file_name;
			rtn = CCommon::MoveAFile(m_hWnd, source_file, wstring(folderPickerDlg.GetPathName()));
		}
		if (rtn == 0)
		{
			//如果文件移动成功，同时从播放列表中移除
			if (m_items_selected.size() > 1)
				theApp.m_player.RemoveSongs(m_items_selected);
			else
				theApp.m_player.RemoveSong(m_item_selected);
			ShowPlayList();
			UpdatePlayPauseButton();
			DrawInfo(true);
		}
	}
}


afx_msg LRESULT CMusicPlayerDlg::OnOpenFileCommandLine(WPARAM wParam, LPARAM lParam)
{
	wstring cmd_line = CCommon::GetStringFromClipboard();
	if (cmd_line.empty())
		return 0;
	vector<wstring> files;
	CCommon::DisposeCmdLine(wstring(cmd_line), files);
	theApp.m_player.OpenFiles(files);
	return 0;
}


void CMusicPlayerDlg::OnFormatConvert()
{
	// TODO: 在此添加命令处理程序代码
	if (!theApp.m_format_convert_dialog_exit)
		return;
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
	m_pFormatConvertDlg = new CFormatConvertDlg(m_items_selected);
	m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
	m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


void CMusicPlayerDlg::OnFormatConvert1()
{
	// TODO: 在此添加命令处理程序代码
	if (!theApp.m_format_convert_dialog_exit)
		return;
	CCommon::DeleteModelessDialog(m_pFormatConvertDlg);
	m_pFormatConvertDlg = new CFormatConvertDlg;
	m_pFormatConvertDlg->Create(IDD_FORMAT_CONVERT_DIALOG);
	m_pFormatConvertDlg->ShowWindow(SW_SHOW);
}


afx_msg LRESULT CMusicPlayerDlg::OnSettingsApplied(WPARAM wParam, LPARAM lParam)
{
	COptionsDlg* pOptionsDlg = (COptionsDlg*)wParam;
	if (pOptionsDlg == nullptr)
		return 0;
	ApplySettings(*pOptionsDlg);
	return 0;
}


void CMusicPlayerDlg::OnRecorder()
{
	// TODO: 在此添加命令处理程序代码
	CRecorderDlg dlg;
	dlg.DoModal();
}


afx_msg LRESULT CMusicPlayerDlg::OnAlbumCoverDownloadComplete(WPARAM wParam, LPARAM lParam)
{
	//由于此函数放到线程中处理时，拉伸图片的处理CDrawCommon::BitmapStretch有一定的概率出错，原因未知
	//导致专辑封面背景是黑色的，因此通过发送消息放到主线程中处理
	theApp.m_player.AlbumCoverGaussBlur();
	return 0;
}


void CMusicPlayerDlg::OnColorizationColorChanged(DWORD dwColorizationColor, BOOL bOpacity)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//响应主题颜色改变消息
	ThemeColorChanged();

	CDialog::OnColorizationColorChanged(dwColorizationColor, bOpacity);
}


void CMusicPlayerDlg::OnSupportedFormat()
{
	// TODO: 在此添加命令处理程序代码
	CSupportedFormatDlg dlg;
	dlg.DoModal();
}


void CMusicPlayerDlg::OnSwitchUi()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pUI == &m_ui)
		m_pUI = &m_ui2;
	else
		m_pUI = &m_ui;

	DrawInfo(true);
	SetThumbnailClipArea();
	m_ui.UpdateRepeatModeToolTip();
	m_ui2.UpdateRepeatModeToolTip();
	m_pUI->UpdateToolTipPosition();
}

void CMusicPlayerDlg::OnVolumeUp()
{
	if (m_miniModeDlg.m_hWnd == NULL)
		theApp.m_player.MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	else
		m_miniModeDlg.SetVolume(true);
}

void CMusicPlayerDlg::OnVolumeDown()
{
	if (m_miniModeDlg.m_hWnd == NULL)
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	else
		m_miniModeDlg.SetVolume(false);
}


afx_msg LRESULT CMusicPlayerDlg::OnNotifyicon(WPARAM wParam, LPARAM lParam)
{
	m_notify_icon.OnNotifyIcon(lParam, m_miniModeDlg.m_hWnd);

	if (lParam == WM_LBUTTONUP && m_miniModeDlg.m_hWnd == NULL)
	{
#ifndef COMPILE_IN_WIN_XP
		m_pTaskbar->ThumbBarAddButtons(m_hWnd, 3, m_thumbButton);	//重新添加任务栏缩略图按钮
		SetThumbnailClipArea();		//重新设置任务栏缩略图
#endif
		UpdatePlayPauseButton();
	}

	return 0;
}


void CMusicPlayerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}


void CMusicPlayerDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(theApp.m_general_setting_data.minimize_to_notify_icon)
		this->ShowWindow(HIDE_WINDOW);
	else
		CDialog::OnCancel();
}


void CMusicPlayerDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	if (m_miniModeDlg.m_hWnd == NULL)
	{
		CDialog::OnCancel();
	}
	else
	{
		::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, ID_MINI_MODE_EXIT, NULL);
	}
}


void CMusicPlayerDlg::OnMinimodeRestore()
{
	// TODO: 在此添加命令处理程序代码
	if (m_miniModeDlg.m_hWnd == NULL)
	{
		OnMiniMode();
	}
	else
	{
		::SendMessage(m_miniModeDlg.m_hWnd, WM_COMMAND, IDOK, NULL);
	}
}


void CMusicPlayerDlg::OnAppCommand(CWnd* pWnd, UINT nCmd, UINT nDevice, UINT nKey)
{
	// 该功能要求使用 Windows 2000 或更高版本。
	// 符号 _WIN32_WINNT 和 WINVER 必须 >= 0x0500。

	//响应多媒体键
	switch (nCmd)
	{
	case APPCOMMAND_MEDIA_PLAY_PAUSE:
		OnPlayPause();
		break;
	case APPCOMMAND_MEDIA_PREVIOUSTRACK:
		OnPrevious();
		break;
	case APPCOMMAND_MEDIA_NEXTTRACK:
		OnNext();
		break;
	case APPCOMMAND_MEDIA_STOP:
		OnStop();
		break;
	default:
		break;
	}

	CDialog::OnAppCommand(pWnd, nCmd, nDevice, nKey);
}
