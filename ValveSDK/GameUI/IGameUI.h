#ifndef IGAMEUI_H
#define IGAMEUI_H

#ifdef _WIN32
#pragma once
#endif

class IGameUI : public IBaseInterface
{
public:
	virtual void Initialize(CreateInterfaceFn *factories, int count) = 0;
	virtual void Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void *system) = 0;
	virtual void Shutdown(void) = 0;
	virtual int ActivateGameUI(void) = 0;
	virtual int ActivateDemoUI(void) = 0;
	virtual int HasExclusiveInput(void) = 0;
	virtual void RunFrame(void) = 0;
	virtual void ConnectToServer(const char *game, int IP, int port) = 0;
	virtual void DisconnectFromServer(void) = 0;
	virtual void HideGameUI(void) = 0;
	virtual bool IsGameUIActive(void) = 0;
	virtual void LoadingStarted(const char *resourceType, const char *resourceName) = 0;
	virtual void LoadingFinished(const char *resourceType, const char *resourceName) = 0;
	virtual void StartProgressBar(const char *progressType, int progressSteps) = 0;
	virtual int ContinueProgressBar(int progressPoint, float progressFraction) = 0;
	virtual void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason = NULL) = 0;
	virtual int SetProgressBarStatusText(const char *statusText) = 0;
	virtual void SetSecondaryProgressBar(float progress) = 0;
	virtual void SetSecondaryProgressBarText(const char *statusText) = 0;
	virtual void TestFunc1(const char *statusText) = 0; // csnz?
	virtual void TestFunc2(const char *statusText) = 0;
	virtual void TestFunc3(const char *statusText) = 0;
	virtual void TestFunc4(const char *statusText) = 0;
	virtual void CreateRoom() = 0;
	virtual void HideAllPanels() = 0;
	virtual void TestFunc7() = 0;
	virtual void TestFunc8() = 0;
	virtual void TestFunc9() = 0;
	virtual void ShowLeaveGameRoomMsg() = 0;
	virtual void ShowInGameMenu() = 0;
	virtual void ShowAddFriendMessageBox(const wchar_t *nickname) = 0;
	virtual void ShowCreateNicknameDlg() = 0;
	virtual void ShowDisconnectMessageBox(const wchar_t *test, int lol) = 0;
	virtual void ShowCafeRegDlg(const wchar_t *test, const wchar_t *reee) = 0;
	virtual void ShowCafeRegDlgNext() = 0; 
	virtual void TestFunc17() = 0;
	virtual void ShowInviteDlg(const wchar_t *test) = 0;
	virtual void ShowRoomPasswordEntry() = 0;
	virtual void ShowRoomPasswordEntry2() = 0;
	virtual void ShowNetworkSettings() = 0;
	virtual void ShowUserInviteDlg() = 0;
	virtual void ShowMessageBox(const char *title, const char *text) = 0;
	virtual void ShowMessageBox(const wchar_t *title, const wchar_t *text) = 0;
	virtual void TestFunc22() = 0;
	virtual void SendMessageToUser(const wchar_t *nickname, const wchar_t *text) = 0;
	virtual void TestFunc24() = 0;
	virtual void TestFunc25() = 0;
	virtual void TestFunc26() = 0;
	virtual void TestFunc27() = 0;
	virtual void TestFunc28() = 0; 
	virtual void TestFunc29() = 0;
};

extern IGameUI* g_pGameUI;

#define GAMEUI_INTERFACE_VERSION "GameUI007"
#endif