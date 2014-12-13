/*
 * ChatMain.cpp
 *
 *  Created on: Dec 8, 2014
 *      Author: ajou
 */

#include "ChatMain.h"
#include "AppResourceId.h"
#include "BasicAppFrame.h"
#include "tabTestFormFactory.h"

using namespace Tizen::App;
using namespace Tizen::Base;
using namespace Tizen::Base::Collection;
using namespace Tizen::Ui;
using namespace Tizen::Ui::Controls;
using namespace Tizen::Ui::Scenes;
using namespace Tizen::Net;
using namespace Tizen::System;

static const int IDC_CHECKServer_SEL = 104;
static const int IDC_CHECKServer_UNSEL = 105;
static const int ID_START = 106;
static const int ID_BUTTON_CLOSE = 108;
static const int PORT_MIN = 1024;
static const int PORT_MAX = 49151;

ChatMain::ChatMain()
   : __pButtonServer(null)
   , __pButtonTcp(null)
   , __pButtonUdp(null)
   , __pEditPort(null)
   , __pClientIp(null)
   , __iport(0)
   , __isProtocol(null)
   , __isServer(null)
   , __pChatForm(null)
{
   // TODO Auto-generated constructor stub

}

ChatMain::~ChatMain() {
   // TODO Auto-generated destructor stub
}

bool
ChatMain::Initialize(void)
{
    // Construct an XML form
    Construct(IDF_ChatMain);

    SetFormBackEventListener(this);

    return true;
}

result
ChatMain::OnInitializing(void)
{
    result r = E_SUCCESS;

   Header* pHeader = GetHeader();
   if (pHeader != null)
   {
      pHeader->SetStyle(HEADER_STYLE_TAB);

      HeaderItem headerItem1;
      headerItem1.Construct(ID_HEADER_ITEM1);
      headerItem1.SetText("calender");
      pHeader->AddItem(headerItem1);

      HeaderItem headerItem2;
      headerItem2.Construct(ID_HEADER_ITEM2);
      headerItem2.SetText("chat");
      pHeader->AddItem(headerItem2);

      HeaderItem headerItem3;
      headerItem3.Construct(ID_HEADER_ITEM3);
      headerItem3.SetText("setting");
      pHeader->AddItem(headerItem3);

      pHeader->AddActionEventListener(*this);
      pHeader->SetItemSelected(1);
   }

    // Get a button via resource ID
    __pButtonServer = static_cast< CheckButton* >(GetControl(IDC_CHECKServer));

    if (__pButtonServer != null)
    {
        __pButtonServer->SetActionId(IDC_CHECKServer_SEL, IDC_CHECKServer_UNSEL);
        __pButtonServer->AddActionEventListener(*this);
    }

    __pButtonTcp = static_cast< CheckButton* >(GetControl(IDC_CHECKTCP));
    __pButtonUdp = static_cast< CheckButton* >(GetControl(IDC_CHECKUDP));

   // __pEditJoinId = static_cast< EditField* >(GetControl(IDC_JOINID));

    //String nickname = __pEditJoinId->GetText();

    //if (__pEditIPAddress == null)
    //{
   //   __pEditIPAddress->AddActionEventListener(*this);
    //    r = E_FAILURE;
    //}

    //__pEditPort = static_cast< EditField* >(GetControl(IDC_EDITPort));



    Footer* pFooter = GetFooter();
    if (pFooter != null)
    {
        pFooter->SetStyle(FOOTER_STYLE_BUTTON_TEXT);

        __footerItem_Left.Construct(ID_START);
        __footerItem_Left.SetText(L"Connect");
        pFooter->AddItem(__footerItem_Left);
        pFooter->AddActionEventListener(*this);
    }

    SetDefaultValues();

    return r;
}

void
ChatMain::OnFormBackRequested(Tizen::Ui::Controls::Form& source)
{
    AppLog("Back Key Pressed");

    UiApp* pApp = UiApp::GetInstance();
    AppAssert(pApp);

    pApp->Terminate();
}

void
ChatMain::SetDefaultValues(void)
{
    if (__pButtonTcp != null)
    {
        __pButtonTcp->SetSelected(true);
    }
}

result
ChatMain::OnTerminating(void)
{
    result r = E_SUCCESS;

    AppLog("ChatMain::OnTerminating");

    return r;
}

void
ChatMain::OnActionPerformed(const Tizen::Ui::Control& source, int actionId)
{
   SceneManager* pSceneManager = SceneManager::GetInstance();
   AppAssert(pSceneManager);


    ArrayList* pList = new (std::nothrow) ArrayList;
    AppAssert(pList);

    switch (actionId)
    {
   case ID_HEADER_ITEM1:
      pSceneManager->GoForward(ForwardSceneTransition(SCENE_CALENDAR_FORM), pList);
      break;
   case ID_HEADER_ITEM2:
      pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHATMAIN), pList);
      break;
   case ID_HEADER_ITEM3:

      break;
    case IDC_CHECKServer_UNSEL:
        {
            AppLog("Server Button unselected");
            //__pEditIPAddress->SetEnabled(true);
            //__pEditIPAddress->RequestRedraw(true);
            Footer* pFooter = GetFooter();
            pFooter->RemoveItemAt(0);
            __footerItem_Left.SetText(L"Connect");
            pFooter->InsertItemAt(0, __footerItem_Left);
            Draw();
        }
        break;

    case IDC_CHECKServer_SEL:
        {
            AppLog("Server button selected");

            //__pEditIPAddress->SetEnabled(false);
            //__pEditIPAddress->RequestRedraw(true);
            Footer* pFooter = GetFooter();
            pFooter->RemoveItemAt(0);
            __footerItem_Left.SetText(L"Listen");
            pFooter->InsertItemAt(0, __footerItem_Left);
            Draw();
        }
        break;

    case ID_START:
        {
            AppLog("Start Button is clicked! \n");
            StartChat();
        }
        break;

    case ID_BUTTON_CLOSE:
        {
            AppLog("Back Button pressed, close application now");
            Application::GetInstance()->Terminate();
        }
        break;

    default:
        AppLog("default");
        break;
    }
}

void
ChatMain::OnSceneActivatedN(const Tizen::Ui::Scenes::SceneId& previousSceneId,
                                       const Tizen::Ui::Scenes::SceneId& currentSceneId, Tizen::Base::Collection::IList* pArgs)
{

}

void
ChatMain::OnSceneDeactivated(const SceneId& currentSceneId, const SceneId& nextSceneId)
{

}

void
ChatMain::StartChat(void)
{
    result r = E_SUCCESS;

    String port;
    // Get protocol type
    __isProtocol = __pButtonTcp->IsSelected();

    // Get connection type
    __isServer = __pButtonServer->IsSelected();

    if (!__isServer)
    {
        // Get ipAddress and port
        String ipAddress = "172.30.123.178";
        ipAddress = "172.30.123.178";
              //__pEditIPAddress->GetText();

        if (ipAddress.IsEmpty())
        {
            int result;
            MessageBox* pMsgBox = new (std::nothrow) MessageBox();
            pMsgBox->Construct(L"IP Address", L"Please enter IP address", MSGBOX_STYLE_OK);
            pMsgBox->ShowAndWait(result);
            delete pMsgBox;
            return;
        }
        else
        {
            __pClientIp = new (std::nothrow) Ip4Address(ipAddress);

            r = GetLastResult();
            if (IsFailed(r) || __pClientIp == null)
            {
                int result;
                MessageBox* pMsgBox = new (std::nothrow) MessageBox();
                pMsgBox->Construct(L"IP Address", L"Invalid IP address", MSGBOX_STYLE_OK);
                pMsgBox->ShowAndWait(result);
                delete pMsgBox;
                return;
            }
        }
    }

    port = "9000";
          //__pEditPort->GetText();
    if (port.IsEmpty())
    {
        int result;
        MessageBox* pMsgBox = new (std::nothrow) MessageBox();
        pMsgBox->Construct(L"Port", L"Enter Valid User Port Number", MSGBOX_STYLE_OK);
        pMsgBox->ShowAndWait(result);
        delete pMsgBox;
        return;
    }
    else
    {
        int result;
        __iport = -1;
        r = Tizen::Base::Integer::Decode(port, __iport);
        AppLog("Port No is %d", __iport);
        if (__iport < PORT_MIN || __iport > PORT_MAX || r != E_SUCCESS)
        {
            MessageBox* pMsgBox = new (std::nothrow) MessageBox();
            pMsgBox->Construct(L"Port", L"Enter Valid User Port Number", MSGBOX_STYLE_OK);
            pMsgBox->ShowAndWait(result);
            delete pMsgBox;
            return;
        }
    }

    if (__pChatForm != null)
    {
        delete __pChatForm;
        __pChatForm = null;
    }

    SceneManager* pSceneManager = SceneManager::GetInstance();
    AppAssert(pSceneManager);

    ArrayList* pList = new (std::nothrow) ArrayList;
    AppAssert(pList);

    r = pList->Construct();
    TryCatch(r == E_SUCCESS, , "Failed to construct the Array list.");

    r = pList->Add(*(new (std::nothrow) Boolean(__isServer)));
    TryCatch(r == E_SUCCESS, , "Failed to add the client/server to the Array list.");

    if (!__isServer)
    {
        r = pList->Add(*(new (std::nothrow) Ip4Address(*__pClientIp)));
        TryCatch(r == E_SUCCESS, , "Failed to add ipAddress to the Array list.");
    }

    r = pList->Add(*(new (std::nothrow) Integer(__iport)));
    TryCatch(r == E_SUCCESS, , "Failed to add port number to the Array list.");

    r = pList->Add(*(new (std::nothrow) Boolean(__isProtocol)));
    TryCatch(r == E_SUCCESS, , "Failed to add the protocol to the Array list.");

    //pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT_MENU), pList);
    pSceneManager->GoForward(ForwardSceneTransition(SCENE_CHAT), pList);

CATCH:
    if (__pClientIp != null)
    {
        delete __pClientIp;
        __pClientIp = null;
    }
}
