///********************************************************************************************///
/// created by: Assaous oussama                                                                ///
/// date :                                                                                     ///
/// description : this program is for drawing the ladder graph and convert it to an asm code.. /// 
/// for the PIC18F2550 microcontroller to make a simple PLC with 8 inputs and 8 outputs        ///
///                                                                                            ///
///********************************************************************************************///

#include"pch.h"
#include"hexconfig.h"
#include "framework.h"
#include "PLC1.h"
#include <string>
#include <iostream>
#include <fstream>
#include "WindowsX.h"

#define   P_NOTHING   0
#define   P_INPUT     1
#define   P_OUTPUT    2
#define   P_LINE      3
#define   P_JUMP_DOWN 4
#define   P_JUMP_UP   5

using namespace std;

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                
WCHAR szTitle[MAX_LOADSTRING];                  
WCHAR szWindowClass[MAX_LOADSTRING];            
HIMAGELIST hImageList = NULL;

//***all the function that has been used on this program****
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void                draw(HWND, int);
int                 select(HWND, int, int);
string              dec_to_hex(int);
int                 hex_to_dec(string);
void                buildprog(int);
string              InstrProcess(int, int, int, int);
int                 StrToInt(string);
HWND                CreateSimpleToolba(HWND);
void                sendprog();


//**********************************************************
HDC hdc;
PAINTSTRUCT ps;
HANDLE hComm;
int i,j,a;
int mousePosX, mousePosY;
int icounter=0;
int valcount = 0;
int Selected;
WCHAR ins[100], outs[100];

//**********************************************************
HDC oxa ;
HBITMAP oussa;
HWND window;

//------------------------------------------------------------------

class inputs {

public:
    WCHAR name[1000][8];
    int in[1000], out[1000];
    int x[1000], y[1000], x1[1000], y1[1000];
    bool stat[1000];
};

class output {
public:
    WCHAR name[1000][8];
    int in[1000];
    int x[1000], y[1000];  
};

int PowerLine[5][1000];
int jumpdown[5][1000];
int jumpup[5][1000];
inputs inpt;
output oupt;
int SignalCounter=1;

//-------------------------------------------------------------------

///****************************************************************************************************************************///
///                                                                                                                            ///
///  FUNCTION : wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR  lpCmdLine,_In_ int nCmdShow)   ///
///                                         OUTPUT : THE MAIN FUNCTION                                                         ///
///                                                                                                                            ///
///****************************************************************************************************************************///

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    
    PowerLine[0][0] = 50;
    PowerLine[1][0] = 100;
    PowerLine[2][0] = 700;//200
    PowerLine[3][0] = 100;
    PowerLine[4][0] = 1;

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Placez le code ici.

    // Initialise les chaînes globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PLC1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Effectue l'initialisation de l'application :
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDI_ICON2));

    MSG msg;

    // Boucle de messages principale :
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
///*****************************************************************************************************************************///

///************************************************///
///                                                ///
///          FONCTION : MyRegisterClass()          ///
///         OUTPUT : REGESTER THE FUNCTION         ///
///                                                ///
///************************************************///
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));// MAKEINTRESOURCE(IDI_SMALL)

    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PLC1);// MAKEINTRESOURCEW(IDC_PLC1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));// MAKEINTRESOURCE(IDI_SMALL)

    return RegisterClassExW(&wcex);
}
///************************************************///

///*********************************************************///
///                                                         ///
///         FONCTION : InitInstance(HINSTANCE, int)         ///
///   OUTPUT : REGESTER THE HANDLER AND CREAT THE WINDOW    ///
///                                                         ///
///*********************************************************///
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Stocke le handle d'instance dans la variable globale

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
    
  
   return TRUE;
}
///*********************************************************///

///***************************************************************************************///
///                                                                                       ///
///                   FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)                      ///
///                     OUTPUT : HANDLE THE MAIN FUNCTION MESSEGES                        ///
///                                                                                       ///
///***************************************************************************************///
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {

    
    case WM_CREATE:
    {  
        
        oxa = CreateCompatibleDC(hdc);
        oussa = LoadBitmap(
        hInst,
        MAKEINTRESOURCE(IDB_BITMAP1)
        );
        SelectObject(oxa, oussa);
        CreateSimpleToolba(hWnd);
    }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            if (wmId == BOT1 || wmId == BOT2 || wmId == BOT3 || wmId == BOT4 || wmId == BOT5 || wmId == BOT6)
            {
                draw(hWnd, wmId);
            }
            // Analyse les sélections de menu :
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case AS:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);

                InvalidateRect(hWnd, NULL, FALSE);
                hdc = BeginPaint(hWnd, &ps);

                MoveToEx(hdc, 100, 100, NULL);
                LineTo(hdc, 200, 200);
               // LineTo(hdc, 150, 50);
                EndPaint(hWnd, &ps); 
               
            }
             break;

            case ID_BUILD:
            {
                buildprog(0);
                
            }
                break;
            case ID_SEND :
            {
                sendprog();
            }
                break;
            default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
        
        PAINTSTRUCT ps;
        InvalidateRect(hWnd, NULL, TRUE);//

        i = 0;
        int m = 0;
        while (inpt.x[i] != NULL)
        {
            InvalidateRect(hWnd, NULL, FALSE);
            hdc = BeginPaint(hWnd, &ps);
            while (inpt.name[i][m] != 0x00)
            {
                m++;
            }

            ins[1] = inpt.in[i]+0x30;
            outs[1] = inpt.out[i]+0x30;
            ins[0] = 'i';
            outs[0] = 'o';

            TextOut(hdc, inpt.x[i] +9 -( m*4 ), inpt.y[i] - 35, inpt.name[i], 8);//LPCWSTR
           // TextOut(hdc, inpt.x[i] + 9 - (m * 4), inpt.y[i] - 35, ins, 8);//LPCWSTR
           // TextOut(hdc, inpt.x[i] + 30 - (m * 4), inpt.y[i] - 35, outs, 8);//LPCWSTR
            EndPaint(hWnd, &ps);
            i++;
        }
      

        i = 0;
        m = 0;
        while (oupt.x[i] != NULL)
        {
            InvalidateRect(hWnd, NULL, FALSE);
            hdc = BeginPaint(hWnd, &ps);
            while (oupt.name[i][m] != 0x00)
            {
                m++;
            }
            TextOut(hdc, oupt.x[i] + 9 - (m * 4), oupt.y[i] - 35, oupt.name[i], 8);//LPCWSTR
            EndPaint(hWnd, &ps);
            i++;
        }

        InvalidateRect(hWnd, NULL, FALSE);
        HDC hdc = BeginPaint(hWnd, &ps);
             
                i = 0;
                while (PowerLine[0][i] != NULL && PowerLine[1][i] != NULL)
                {
                    MoveToEx(hdc, PowerLine[0][i], PowerLine[1][i],NULL);
                    LineTo(hdc, PowerLine[2][i], PowerLine[3][i]);

                    MoveToEx(hdc, PowerLine[0][i], PowerLine[1][i]-50, NULL);
                    LineTo(hdc, PowerLine[0][i], PowerLine[1][i]+50);
                    i++;
                }


                i = 0;
                while (jumpdown[0][i] != NULL && jumpdown[1][i] != NULL)
                {

                    InvalidateRect(hWnd, NULL, FALSE);
                    hdc = BeginPaint(hWnd, &ps);
                    MoveToEx(hdc, jumpdown[0][i], jumpdown[1][i], NULL);
                    LineTo(hdc, jumpdown[0][i], jumpdown[3][i]);
                    LineTo(hdc, jumpdown[2][i], jumpdown[3][i]);

                    EndPaint(hWnd, &ps);
                    i++;
                }
                i = 0;
                while (jumpup[0][i] != NULL && jumpup[1][i] != NULL)
                {

                    InvalidateRect(hWnd, NULL, FALSE);
                    hdc = BeginPaint(hWnd, &ps);
                    MoveToEx(hdc, jumpup[0][i], jumpup[1][i], NULL);
                    LineTo(hdc, jumpup[2][i], jumpup[1][i]);
                    LineTo(hdc, jumpup[2][i], jumpup[3][i]);

                    EndPaint(hWnd, &ps);
                    i++;
                }
         EndPaint(hWnd, &ps);
                i = 0;
                while (inpt.x1[i] != NULL  && inpt.y1[i] != NULL )
                {
                   InvalidateRect(hWnd, NULL, FALSE);
                    hdc = BeginPaint(hWnd, &ps);
                    MoveToEx(hdc, inpt.x[i]+14, inpt.y[i], NULL);
                    LineTo(hdc, inpt.x1[i], inpt.y1[i]);
                    if (inpt.stat[i] == TRUE) a = 0;
                    else a = 21;
                    

                    //SelectObject(oxa, oussa);
                    BitBlt(
                        hdc,
                        inpt.x[i],
                        inpt.y[i]-13,
                        20,//47
                        26,//47
                        oxa,
                        a,//a
                        0,
                        SRCCOPY
                    );

                    EndPaint(hWnd, &ps);
                    i++;
                }

                //*****
                i = 0;
                while (oupt.x[i] != NULL && oupt.y[i] != NULL )
                {
                    InvalidateRect(hWnd, NULL, FALSE);
                    hdc = BeginPaint(hWnd, &ps);

                    MoveToEx(hdc, oupt.x[i], oupt.y[i], NULL);
                    LineTo(hdc, oupt.x[i] + 50, oupt.y[i]);

                    MoveToEx(hdc, oupt.x[i]+50, oupt.y[i]-5, NULL);
                    LineTo(hdc, oupt.x[i]+ 50, oupt.y[i]+5);


                   

                    //SelectObject(oxa, oussa);
                    BitBlt(
                        hdc,
                        oupt.x[i],
                        oupt.y[i] - 13,
                        20,//47
                        26,//47
                        oxa,
                        105,
                        0,
                        SRCCOPY
                    );

                    EndPaint(hWnd, &ps);
                    i++;
                }

               
        }
        break;
    case WM_LBUTTONDOWN :
      {
        mousePosX = GET_X_LPARAM(lParam);
        mousePosY = GET_Y_LPARAM(lParam);
        Selected = select(hWnd,mousePosX, mousePosY);
      }
        break;
    case WM_DESTROY:     
            PostQuitMessage(0);   
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

///***************************************************************************************///

///*****************************************************************************///
///                                                                             ///
///    FUNCTION : About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)  ///
///              OUTPUT : GIVE INFORMATION ABOUT THE PROGRAM                    ///
///                                                                             ///
///*****************************************************************************///
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

    LPPAINTSTRUCT lpPaint = NULL;
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            
            return (INT_PTR)TRUE;
        }
        break;
    
    }
    return (INT_PTR)FALSE;
}

///*****************************************************************************///

///***********************************************************///
///                                                           ///
///        FUINCTION : HWND CreateSimpleToolba(HWND)          ///
///             OUTPUT : CREAT THE TOOL BAR                   ///
///                                                           ///
///***********************************************************///
//HIMAGELIST g_hImageList = NULL;

HWND CreateSimpleToolba(HWND hWndParent)
{
    // Declare and initialize local constants.
    const int ImageListID = 0;
    const int numButtons = 6;
    const int bitmapSize = 48;

    const DWORD buttonStyles =  BTNS_AUTOSIZE | BTNS_GROUP;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        TBSTYLE_TRANSPARENT | WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0, //| WS_CHILD | TBSTYLE_WRAPABLE
        hWndParent, NULL,0 , NULL);

    if (hWndToolbar == NULL)
        return NULL;

    hImageList = ImageList_LoadImage(hInst, MAKEINTRESOURCE(IDB_BITMAP2), 48, 0, CLR_DEFAULT, IMAGE_BITMAP, LR_LOADTRANSPARENT);//LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_SHARED | LR_LOADTRANSPARENT

    // Set the image list.
    SendMessage(hWndToolbar, TB_SETIMAGELIST,
        (WPARAM)ImageListID,
        (LPARAM)hImageList);

    // Initialize button info.
    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

    TBBUTTON tbButtons[numButtons] =
    {
        { 0, BOT1,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL },//MAKELONG(STD_FILENEW,  ImageListID)   (INT_PTR)L"New"  TBSTATE_ENABLED
        { 1, BOT2,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL},// MAKELONG(STD_FILEOPEN, ImageListID) (INT_PTR)L"Open"
        { 2, BOT3,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL}, //MAKELONG(STD_FILESAVE, ImageListID)   (INT_PTR)L"Save"
        { 3, BOT4,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL },
        { 4, BOT5,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL },
        { 5, BOT6,  TBSTATE_ENABLED, buttonStyles, {0}, 0, NULL }
    };

    // Add buttons.
    
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
   
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);
    

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
   
    ShowWindow(hWndToolbar, TRUE);
   

    return hWndToolbar;
}
///***********************************************************///

///***********************************************************///
///                                                           ///
///            FUNCTION : void draw(HWND,int)                 ///
///     OUTPUT : DRAW THE INPUT.. OUTPUT .. LINE ...          ///
///                                                           ///
///***********************************************************///

void draw(HWND hw ,int ButClicked)
{
    switch (ButClicked)
    {
       case BOT1:
           
       {   
              
           switch (Selected)
           {
               case P_LINE:
               {
                   i = 0;
                   while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL )
                   {
                       i++;
                   }
                   SignalCounter++;

                   inpt.name[i][0] = '?';
                  

                   inpt.in[i] = PowerLine[4][icounter];
                   inpt.out[i] = SignalCounter;
                   inpt.x[i] = PowerLine[0][icounter] + 40;//10
                   inpt.y[i] = PowerLine[1][icounter] ;
                   inpt.x1[i] = PowerLine[2][icounter];
                   inpt.y1[i] = PowerLine[3][icounter];

                   PowerLine[2][icounter]= inpt.x[i];
                   PowerLine[3][icounter]= inpt.y[i];
                   inpt.stat[i] = TRUE;

                   SendMessage(hw, WM_PAINT, 0, 0);
                   return;              
               }
               break;
               case P_INPUT:
               {

                   i = 0;
                   while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL )
                   {
                       i++;
                   }
                   SignalCounter++;
                   inpt.name[i][0] = '?';
                   inpt.in[i] = inpt.out[icounter];
                   inpt.out[i] = SignalCounter;
                   inpt.x[i] = inpt.x[icounter] +100;//30
                   inpt.y[i] = inpt.y[icounter] ;


                   if (inpt.x1[icounter] < inpt.x[i] + 35)
                   {
                       inpt.x1[i] = inpt.x[i] + 60;//+35
                   }
                   else
                   {
                       inpt.x1[i] = inpt.x1[icounter];
                   }
                   inpt.y1[i] = inpt.y1[icounter];

                   
                   inpt.x1[icounter ] = inpt.x[i];
                   inpt.y1[icounter ] = inpt.y[i];
                   inpt.stat[i] = TRUE;
                   SendMessage(hw, WM_PAINT, 0, 0);
                   return;
                  
               }
               break;
               case P_JUMP_DOWN:
               {
                   i = 0;
                   while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL)
                   {
                       i++;
                   }
                   SignalCounter++;
                   inpt.name[i][0] = '?';
                   inpt.in[i] = jumpdown[4][icounter];
                   inpt.out[i] = SignalCounter;
                   inpt.x[i] = jumpdown[0][icounter]+40;//40
                   inpt.y[i] = jumpdown[3][icounter];

                   inpt.x1[i] = jumpdown[2][icounter] + 50;//+20
                   inpt.y1[i] = jumpdown[3][icounter] ;

                   inpt.stat[i] = TRUE;
                   SendMessage(hw, WM_PAINT, 0, 0);
                   return;
               }
                   break;
               case P_NOTHING:
                   return ;
           }
       }
       break;
       case BOT2:
       {
           
           switch (Selected)
           {
             case P_LINE:
               {
                 i = 0;
                 while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL)
                 {
                   i++;
                 }
                 SignalCounter++;
                 inpt.name[i][0] = '?';
                 inpt.in[i] = PowerLine[4][icounter];
                 inpt.out[i] = SignalCounter;
                 inpt.x[i] = PowerLine[0][icounter] + 40;//+10
                 inpt.y[i] = PowerLine[1][icounter];
                 inpt.x1[i] = PowerLine[2][icounter];
                 inpt.y1[i] = PowerLine[3][icounter];

                 PowerLine[2][icounter] = inpt.x[i];
                 PowerLine[3][icounter] = inpt.y[i];
                 inpt.stat[i] = FALSE;
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;
              
               }
               break;
               case P_INPUT:
               {

                 i = 0;
                 while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL)
                 {
                   i++;
                 }
                 SignalCounter++;
                 inpt.name[i][0] = '?';
                 inpt.in[i] = inpt.out[icounter];
                 inpt.out[i] = SignalCounter;
                 inpt.x[i] = inpt.x[icounter] + 100;//+30
                 inpt.y[i] = inpt.y[icounter];


                 if (inpt.x1[icounter] < inpt.x[i] + 35)
                 {
                   inpt.x1[i] = inpt.x[i] + 60;
                 }
                 else
                 {
                   inpt.x1[i] = inpt.x1[icounter];
                 }
                 inpt.y1[i] = inpt.y1[icounter];


                 inpt.x1[icounter] = inpt.x[i];
                 inpt.y1[icounter] = inpt.y[i];
                 inpt.stat[i] = FALSE;
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;

               }
               break;
               case P_JUMP_DOWN:
               {
                 i = 0;
                 while (inpt.in[i] != NULL && inpt.out[i] != NULL && inpt.x[i] != NULL)
                 {
                   i++;
                 }
                 SignalCounter++;
                 inpt.name[i][0] = '?';
                 inpt.in[i] = jumpdown[4][icounter];
                 inpt.out[i] = SignalCounter;
                 inpt.x[i] = jumpdown[0][icounter]+40;//-5
                 inpt.y[i] = jumpdown[3][icounter];

                 inpt.x1[i] = jumpdown[2][icounter] + 40;//+20
                 inpt.y1[i] = jumpdown[3][icounter];
                 inpt.stat[i] = FALSE;
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;

               }
               break;
               case P_NOTHING:
               return;
           }
       }
       break;
       case BOT3:
       {

           switch (Selected)
           {   case P_LINE:
               {
              
                 i = 0;
                 while (jumpdown[0][i] != NULL && jumpdown[1][i] != NULL)
                 {
                   i++;
                 }
                 jumpdown[0][i] = PowerLine[0][icounter];
                 jumpdown[1][i] = PowerLine[1][icounter];
                 jumpdown[2][i] = jumpdown[0][i] + 50;//15
                 jumpdown[3][i] = jumpdown[1][i] + 50;
                 jumpdown[4][i] = PowerLine[4][icounter];
           
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;
               }
               break;
               case P_INPUT:
               { 
                 i = 0;
                 while (jumpdown[0][i] != NULL && jumpdown[1][i] != NULL)
                 {
                      i++;
                 }
                 jumpdown[0][i] = inpt.x[icounter]+60;
                 jumpdown[1][i] = inpt.y[icounter];
                 jumpdown[2][i] = jumpdown[0][i] + 50;
                 jumpdown[3][i] = jumpdown[1][i] + 50;
                 jumpdown[4][i] = inpt.out[icounter];
               
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;
               }
               break;
               case P_JUMP_DOWN :
               {
                   i = 0;
                   while (jumpdown[0][i] != NULL && jumpdown[1][i] != NULL)
                   {
                       i++;
                   }
                   jumpdown[0][i] = jumpdown[0][icounter] ;
                   jumpdown[1][i] = jumpdown[3][icounter];
                   jumpdown[2][i] = jumpdown[0][i] + 50;
                   jumpdown[3][i] = jumpdown[1][i] + 50;
                   jumpdown[4][i] = jumpdown[4][icounter];

                   SendMessage(hw, WM_PAINT, 0, 0);
                   return;

               }
               break;
           }

       }
       break;
       case BOT4:
       {
           switch (Selected)
           {
             case P_LINE:
             {

               i = 0;
               while (jumpup[0][i] != NULL && jumpup[1][i] != NULL)
               {
                   i++;
               }
               jumpup[0][i] = PowerLine[0][icounter];
               jumpup[1][i] = PowerLine[1][icounter];
               jumpup[2][i] = jumpup[0][i] + 15;//10
               jumpup[3][i] = jumpup[1][i] - 50;
               jumpup[4][i] = PowerLine[4][icounter];

               SendMessage(hw, WM_PAINT, 0, 0);
               return;
             }
             case P_INPUT:
             {
                 i = 0;
                 while (jumpup[0][i] != NULL && jumpup[1][i] != NULL)
                 {
                     i++;
                 }
                 jumpup[0][i] = inpt.x[icounter] +15;
                 jumpup[1][i] = inpt.y[icounter];
                 jumpup[2][i] = jumpup[0][i] + 45;
                 jumpup[3][i] = jumpup[1][i] -50;

                // i = icounter;
                 int k = icounter;
                 int v= 0;
                 switch (select(hw, jumpup[2][i], jumpup[3][i]) )
                 {
                     
                    case P_INPUT:
                    {

                        while (jumpdown[0][v] != NULL)
                        {
                            if (jumpdown[4][v] == inpt.out[icounter])
                            {
                                jumpup[0][i] = 0;
                                jumpup[1][i] = 0;
                                jumpup[2][i] = 0;
                                jumpup[3][i] = 0;
                                SendMessage(hw, WM_PAINT, 0, 0);
                                return;
                            }
                            v++;
                        }
                       
                            inpt.out[k] = inpt.out[icounter];
                            jumpup[4][i] = inpt.out[icounter];
                         
                    }
                    break;
                    case P_NOTHING:
                    {
                       jumpup[4][i] = inpt.out[icounter];
                    }
                    break;

                   //  jumpup[4][i] = inpt.out[icounter];
                 }
                 SendMessage(hw, WM_PAINT, 0, 0);
                 return;
             }
           }

           Selected = P_NOTHING;
       }
       break;
       case BOT5:
       {
           Selected = P_NOTHING;
       }
       break;
       case BOT6:
       {  switch (Selected)
          {
            case P_LINE:
            { 
                i = 0;
                while (oupt.in[i] != NULL && oupt.x[i] != NULL)
                {
                    i++;
                }
                oupt.name[i][0] = '?';
                oupt.in[i] = PowerLine[4][icounter];
                oupt.x[i] = 650;//PowerLine[2][icounter]-50;
                oupt.y[i] = PowerLine[3][icounter];

                PowerLine[2][icounter] = oupt.x[i];
                PowerLine[3][icounter] = oupt.y[i];
                SendMessage(hw, WM_PAINT, 0, 0);
                return;
            }
            break;
            case P_INPUT:
            { 
                i = 0;
                while (oupt.in[i] != NULL && oupt.x[i] != NULL)
                {
                    i++;
                }
                oupt.name[i][0] = '?';
                oupt.in[i] = inpt.out[icounter];//inpt.in[icounter]
                oupt.x[i] = 650; //inpt.x1[icounter] - 50;
                oupt.y[i] = inpt.y1[icounter];

                inpt.x1[icounter] = oupt.x[i];
                inpt.y1[icounter] = oupt.y[i];
                SendMessage(hw, WM_PAINT, 0, 0);
                return;

            }
            break;
            case P_JUMP_DOWN:
            {
                i = 0;
                while (oupt.in[i] != NULL && oupt.x[i] != NULL)
                {
                    i++;
                }
                oupt.name[i][0] = '?';
                oupt.in[i] = jumpdown[4][icounter];
                oupt.x[i] = 650; //inpt.x1[icounter] - 50;
                oupt.y[i] = jumpdown[3][icounter];

                jumpdown[2][icounter] = oupt.x[i];
                jumpdown[3][icounter] = oupt.y[i];
                SendMessage(hw, WM_PAINT, 0, 0);
                return;
            }
            break;
          }
          
           
       }
       break;
       default:
       break;

    }

}

///***********************************************************///

///***********************************************************///
///                                                           ///
///      FUNCTION : select(HWND hw ,int mousx,int mousy)      ///
///    OUTPUT : SELECT THE OBJECT THAT HAS BEEN  CLICKED      ///
///                                                           ///
///***********************************************************///
int select(HWND hw ,int mousx,int mousy)
{
    j = 0;
    while (PowerLine[0][j] != NULL  )
    {
        if ((mousx >= PowerLine[0][j] && mousx <= PowerLine[2][j]) && (mousy >= (PowerLine[1][j]-2) && mousy <= (PowerLine[3][j]+2)))
        {
            DestroyWindow(window);
         //   LOGBRUSH lb;
            SendMessage(hw, WM_PAINT, 0, 0);
          
            PAINTSTRUCT p;
            InvalidateRect(hw, NULL, FALSE);
            HDC hd = BeginPaint(hw, &p);
            //**
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
            
            HGDIOBJ hOldPen = SelectObject(hd, hPen);
            SelectObject(hd, hPen);
 
            //**
            MoveToEx(hd, PowerLine[0][j], PowerLine[1][j], NULL);
            LineTo(hd, PowerLine[2][j], PowerLine[3][j]);
           
            SelectObject(hd, hOldPen);
            DeleteObject(hPen);
            EndPaint(hw, &p);
            icounter = j;
            return P_LINE;
          
        }

        j++;
    }

    j = 0;
    while (inpt.x1[j] != NULL && inpt.y1[j] != NULL)
    {
        if ((mousx >= inpt.x[j]+14 && mousx <= inpt.x1[j]) && (mousy >= (inpt.y[j]-2) && mousy <= (inpt.y1[j]+2)))
        {
            DestroyWindow(window);
            //LOGBRUSH lb;
            SendMessage(hw, WM_PAINT, 0, 0);
            PAINTSTRUCT p;
            InvalidateRect(hw, NULL, FALSE);
            HDC hd = BeginPaint(hw, &p);
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));

            HGDIOBJ hOldPen = SelectObject(hd, hPen);
            SelectObject(hd, hPen);
            MoveToEx(hd, inpt.x[j] + 16, inpt.y[j], NULL);
            LineTo(hd, inpt.x1[j]+2, inpt.y1[j]);
           
            SelectObject(hd, hOldPen);
            DeleteObject(hPen);
            EndPaint(hw, &p);
           
            icounter = j;
            return P_INPUT;
        }

        if ((mousx >= inpt.x[j]  && mousx <= inpt.x[j]+14) && (mousy >= (inpt.y[j] - 13) && mousy <= (inpt.y1[j] + 13)))
        {
           DestroyWindow(window);
           
           
            
            
            SendMessage(hw, WM_PAINT, 0, 0);
          
            window = CreateWindow(L"EDIT", inpt.name[j], ES_UPPERCASE |ES_CENTER | WS_BORDER | WS_CHILD | WS_VISIBLE, inpt.x[j] - 20, inpt.y[j] - 35, 60, 20, hw, (HMENU)1, NULL, NULL);
            // WS_BORDER | WS_CHILD | WS_VISIBLE
            //TextOut(hdc, 50, 100, inpt.name[j], 5);//LPCWSTR
            InvalidateRect(hw, NULL, FALSE);
            hdc = BeginPaint(hw, &ps);

            if (inpt.stat[j] == TRUE) a = 126;
            else a = 147;
           
            BitBlt(
                hdc,
                inpt.x[j],
                inpt.y[j] - 13,
                20,//47
                26,//47
                oxa,
                a,
                0,
                SRCCOPY
            );
           
       
            icounter = j;
            return P_INPUT;
        }

        j++;
    }

    j = 0; 
    while (jumpdown[0][j] != NULL && jumpdown[1][j] != NULL)
    {
        if ((mousx >= jumpdown[0][j]-2 && mousx <= jumpdown[0][j] + 2 && mousy >= jumpdown[1][j] && mousy <= jumpdown[3][j]) || (mousy >= jumpdown[3][j]-2 && mousy <= jumpdown[3][j]+2 && mousx >= jumpdown[0][j] && mousx <= jumpdown[2][j]))
        {
            DestroyWindow(window);
            SendMessage(hw, WM_PAINT, 0, 0);
            InvalidateRect(hw, NULL, FALSE);
            hdc = BeginPaint(hw, &ps);
            HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));

            HGDIOBJ hOldPen = SelectObject(hdc, hPen);
            SelectObject(hdc, hPen);
            MoveToEx(hdc, jumpdown[0][j], jumpdown[1][j], NULL);
            LineTo(hdc, jumpdown[0][j], jumpdown[3][j]);
            LineTo(hdc, jumpdown[2][j], jumpdown[3][j]);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
            EndPaint(hw, &ps);
            icounter = j;
            return P_JUMP_DOWN;
        }
        j++;
    }

    j = 0;

    while (oupt.x[j] != NULL && oupt.y[j] != NULL)
    {
        if ((mousx >= oupt.x[j] && mousx <= oupt.x[j] + 20) && (mousy >= (oupt.y[j] - 13) && mousy <= (oupt.y[j] + 13)))
        {
            DestroyWindow(window);
            SendMessage(hw, WM_PAINT, 0, 0);
            window = CreateWindow(L"EDIT", oupt.name[j], ES_UPPERCASE | ES_CENTER | WS_BORDER | WS_CHILD | WS_VISIBLE, oupt.x[j] - 20, oupt.y[j] - 35, 60, 20, hw, (HMENU)1, NULL, NULL);
            //SendMessage(hw, WM_PAINT, 0, 0);
            InvalidateRect(hw, NULL, FALSE);
            hdc = BeginPaint(hw, &ps);

            BitBlt(
                hdc,
                oupt.x[j],
                oupt.y[j] - 13,
                20,//47
                26,//47
                oxa,
                168,
                0,
                SRCCOPY
            );

            icounter = j;
            return P_OUTPUT;
        }
        j++;
    }

    if (window)
    {
        switch (Selected)
        {
        case P_INPUT:
        {
            GetWindowTextW(window, inpt.name[icounter], 8);//LPWSTR                   
        }
            break;
        case P_OUTPUT:
        {
            GetWindowTextW(window, oupt.name[icounter], 8);//LPWSTR                  
        }
            break;
        }
        DestroyWindow(window);
        window = FALSE;
    }
  SendMessage(hw, WM_PAINT, 0, 0);
  
   // TextOut(hdc, inpt.x[icounter], inpt.y[icounter]+13, inpt.name, 7);//LPCWSTR
  
    return P_NOTHING;
}
///***********************************************************///

///**************************************************************///
///                                                              ///
///              FUNCTION : buildprog(int z)                     ///
///  OUTPUT : PROCESS THE LADDER GRAPH AND CREATE  THE HEX FILE  ///
///                                                              ///
///**************************************************************///

void buildprog(int z)
{

    string outf[1000];
    int hexcount=0;
    int instr = 0;
    //ifstream myfile("tes.txt");
    ofstream myfile;

   //*************
    int instrcount = 54;// 50;// 48;// 40;
    int subpr = 0;
    int prog = 0;
    int start;
    //*************


    i = 0;
    while (inpt.x[i] != NULL)
    {
        if (inpt.stat[i])
        {
            instrcount += 26;//22; 
         }
        else
        {
            instrcount += 30;//28;
        }

        i++;
    }

    i = 0;
    while (oupt.x[i] != NULL)
    {
        instrcount += 8;//6;

        i++;
    }


    start = (0x7ffe - instrcount)/2;
    subpr = ((0x7ffe - instrcount) +14)/2;
    prog = ((0x7ffe - instrcount) + 32)/2;


    if (instrcount > 0x7842)
    {
           
        MessageBox(
            NULL,
            L"ther is no more space ",
            L"BUILD ",
            MB_ICONASTERISK | MB_OK  //MB_ICONEXCLAMATION
        );
        return;
    }
    

    myfile.open("tes.hex");
    if (myfile.is_open())
    {

    
    i = 0;

    int s;
    int sig[1000];
    sig[1] = 0;



   /* //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //GOTO PROG

    instr = prog & 0xff;
    instr = instr | GOTO;
    outf[hexcount] = dec_to_hex(instr);
    // myfile << "CALL subpr : " << dec_to_hex(instr) <<"  1sw" <<endl;
    hexcount++;
    instr = 0xf000;
    instr = instr | (prog >> 8);//instr = instr | (subpr >> 8);
   // myfile << "CALL subpr : " << dec_to_hex(instr) << "  2sw" << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
    //MOVLW 0xFF 
    instr = MOVLW;
    instr = instr | 0xFF;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVWF TRISA
    instr = TRISA & 0xFF;
    instr = instr | MOVWF;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     //MOVLW 0x00 
    instr = MOVLW;
    instr = instr | 0x00;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVWF TRISB
    instr = TRISB & 0xFF;
    instr = instr | MOVWF;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
     //SETF ADCON1

    instr = ADCON1 & 0xff;
    instr = instr | SETF; // clrf r1
   // instr = instr | R1;
    //myfile <<"CLRF R1 : "<< dec_to_hex(instr) << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
     //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //GOTO PROG

    instr = prog & 0xff;
    instr = instr | GOTO;
    outf[hexcount] = dec_to_hex(instr);
    // myfile << "CALL subpr : " << dec_to_hex(instr) <<"  1sw" <<endl;
    hexcount++;
    instr = 0xf000;
    instr = instr | (prog >> 8);//instr = instr | (subpr >> 8);
   // myfile << "CALL subpr : " << dec_to_hex(instr) << "  2sw" << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    //                            SUBPROG                                //
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
    //BTFSC biti
    instr = R1 & 0xff;
    instr = instr | BTFSC;
    instr = instr | (biti << 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BTFSS bits1
    instr = R1 & 0xff;
    instr = instr | BTFSS;
    instr = instr | (bits1 << 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BTFSC bitand
    instr = R1 & 0xff;
    instr = instr | BTFSC;
    instr = instr | (bitand << 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BSF bitand 
    instr = R1 & 0xff;
    instr = instr | BSF;
    instr = instr | (bitand<< 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BTFSC bitand 
    instr = R1 & 0xff;
    instr = instr | BTFSC;
    instr = instr | (bitand<< 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BSF   bits2
    instr = R1 & 0xff;
    instr = instr | BSF;
    instr = instr | (bits2<< 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BTFSS bits2
    instr = R1 & 0xff;
    instr = instr | BTFSS;
    instr = instr | (bits2 << 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //BCF   bits2
    instr = R1 & 0xff;
    instr = instr | BCF;
    instr = instr | (bits2 << 9);
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //RETURN
    instr = RETURN;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //----------------------------END SUBPROG------------------------------
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                 //MOVFF PORTA,Rinpt
    instr = MOVFF;
    instr = instr | PORTA;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    instr = 0xF000;
    instr = instr | Rinpt;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
                 //MOVLW BANK1
    instr = MOVLW;
    instr = instr | BANK1;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVWF BSR
    instr = MOVWF;
    instr = instr | BSR;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    //CLRF 0x00
    //instr = 0x00 & 0xff;
  //  instr = instr | CLRF; // clrf r1
  //  outf[hexcount] = dec_to_hex(instr);

    outf[hexcount]  = InstrProcess(0x00, 0xff, 0, CLRF);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
     //------------------------------//
    int BitS1, adr, bitloc;
    //--------------------------------
    instr = BSF; // BSF S1
    BitS1 = 1;// inpt.in[0];

    adr = int(BitS1 - 1) / 8;
    if (BitS1 % 8 != 0)
        bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
    else bitloc = 7;
    instr = instr | (bitloc << 9);
    instr = instr | adr;
    // myfile << "BSF S2 : " << dec_to_hex(instr) << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  /*  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    //MOVFF PORTA,Rinpt
    instr = MOVFF;
    instr = instr | PORTA;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    instr = 0xF000;
    instr = instr | Rinpt;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVLW BANK1
    instr = MOVLW;
    instr = instr | BANK1;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVWF BSR
    instr = MOVWF;
    instr = instr | BSR;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

    //processing the inputs..
    while (inpt.x[i] != NULL)
    {
        s = inpt.out[i];
        j = 0;
      //  myfile << (char)inpt.name[i][0]<< (char)inpt.name[i][1] << "  in : "<<inpt.in[i]<< "||  out :  "<<inpt.out[i]<< endl;
    /*    while (inpt.x[j] != NULL)
        {
            if (inpt.out[j] == s)
            {

                // if sig[n] = 0 means the signal has  not been  processed 
                // if sig[n] = 1 means the signal has been processed 
                if (sig[inpt.in[j]] == 0)
                {
                    sig[inpt.out[i]] = 1;
                }
                else
                {
                    sig[inpt.out[i]] = 0;
                    break;
                }
            }
            j++;
        } */
        sig[inpt.out[i]] = 1;
        // myfile:cout << "H";
        char OUD[8][2] = {
                             { 'E','0' },
                             { 'E','1' },
                             { 'E','2' },
                             { 'E','3' },
                             { 'E','4' },
                             { 'E','5' },
                             { 'E','6' },
                             { 'E','7' }                      
        };

        if (sig[inpt.out[i]] == 1)
        {

            string oid = "";
            int inptport=0;
            int y = 0;
            while((char)inpt.name[i][y] != ' ' && (char)inpt.name[i][y] != NULL)
            {
                oid = oid + (char)inpt.name[i][y];
                y++;
            }
            y = 0;

           // oid = oid+(char)inpt.name[i][0] + (char)inpt.name[i][1] + (char)inpt.name[i][2];
                // myfile << "input : "<<oid<<endl;
                 
            if (oid == "E0")
                inptport = E0;
            else if (oid == "E1")
                inptport = E1;
            else if (oid == "E2")
                inptport = E2;
            else if (oid == "E3")
                inptport = E3;
            else if (oid == "E4")
                inptport = E4;
            else if (oid == "E5")
                inptport = E5;
            else if (oid == "E6")
                inptport = E6;
            else if (oid == "E7")
                inptport = E7;
            else if (oid[0] == 'M')
            {
                inptport = 10;
                y = 1;
                while ((char)inpt.name[i][y] != ' ' && (char)inpt.name[i][y] != NULL)
                {
                    oid = oid + (char)inpt.name[i][y];
                    y++;
                }
                
                y = 0;
            }
                 int instr =0;



                 //------------------------------//
                 int BitS1, adr, bitloc;
               /*  //------------------------------//
                  instr = BSF; // BSF S1
                 BitS1 = inpt.in[0];

                 adr = int(BitS1 - 1) / 8;
                 if (BitS1 % 8 != 0)
                     bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                 else bitloc = 7;
                 instr = instr | (bitloc << 9);
                 instr = instr | adr;
                 // myfile << "BSF S2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                  hexcount++;
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */





                 
                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 //CLRF R1
                 instr = R1 & 0xff;
                 instr = instr |CLRF; // clrf r1
                // instr = instr | R1;
                 //myfile <<"CLRF R1 : "<< dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 if (!inpt.stat[i])
                 {

                     if (inptport == 10)
                     {
   
                         BitS1 = StrToInt(oid);//inpt.intspec[i];//inpt.in[i];
                         adr = (int(BitS1 - 1) / 8) + valofst;
                         if (BitS1 % 8 != 0)
                             bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                         else bitloc = 7;

                         instr = adr & 0xff;
                         instr = instr | BTG;
                         instr = instr | (bitloc << 9);
                         outf[hexcount] = dec_to_hex(instr);
                         hexcount++;
                     }
                     else
                     {
                         instr = Rinpt & 0xff;
                         instr = instr | BTG;
                         instr = instr | (inptport << 9);
                         // instr = instr | Rinpt;
                        //  myfile << "BTG i : " << dec_to_hex(instr) << endl;
                         outf[hexcount] = dec_to_hex(instr);
                         hexcount++;
                     }
                 }
                 
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 if (inptport == 10)
                 {
                     instr = BTFSC;//BTFSC VAL
                 // int BitS1,adr,bitloc;
                     BitS1 = StrToInt(oid);//inpt.intspec[i];//inpt.in[i];

                     adr = (int(BitS1 - 1) / 8) + valofst;
                     if (BitS1 % 8 != 0)
                         bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                     else bitloc = 7;
                     instr = instr | (bitloc << 9);
                     instr = instr | adr;
                     // myfile << "BTFSC S1 : " << dec_to_hex(instr) << endl;
                     outf[hexcount] = dec_to_hex(instr);
                     hexcount++;
                 }
                 //-------
                 else {
                     instr = Rinpt & 0xff;
                     instr = instr | BTFSC; //  BTFSC i
                     instr = instr | (inptport << 9);
                     // instr = instr | Rinpt;
                     // myfile << "BTFSC i : " << dec_to_hex(instr) << endl;
                     outf[hexcount] = dec_to_hex(instr);
                     hexcount++;
                     }
                
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = R1 & 0xff;
                 instr = instr | BSF; // BSF biti
                 instr = instr | (biti << 9);
                 //instr = instr | R1;
                 //instr = instr | (inptport << 9);
                 //myfile << "BSF biti : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
              

                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = BTFSC;//BTFSC S1
                // int BitS1,adr,bitloc;
                 BitS1 = inpt.in[i];

                 adr = int(BitS1 - 1) / 8;
                 if (BitS1 % 8 != 0)
                     bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                 else bitloc = 7;
                 instr = instr | (bitloc << 9);
                 instr = instr | adr;
                // myfile << "BTFSC S1 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = R1 & 0xff;
                 instr = instr | BSF; // BSF bits1
                 instr = instr | (bits1 << 9);
                // instr = instr | R1;
                 //instr = instr | (inptport << 9);
                 //myfile << "BSF bits1 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = BTFSC;//BTFSC S2
                 //int BitS1, adr, bitloc;
                 BitS1 = inpt.out[i];

                 adr = int(BitS1 - 1) / 8;
                 if (BitS1 % 8 != 0)
                     bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                 else bitloc = 7;
                 instr = instr | (bitloc << 9);
                 instr = instr | adr;
                 //myfile << "BTFSC S2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = BSF; // BSF bits2
                 instr = instr | (bits2 << 9);
                 instr = instr | R1;
                 //instr = instr | (inptport << 9);
               //  myfile << "BSF bits2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
               //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 //CALL subpr

                 instr = subpr & 0xff;
                 instr = instr | CALL;
                 outf[hexcount] = dec_to_hex(instr);
                // myfile << "CALL subpr : " << dec_to_hex(instr) <<"  1sw" <<endl;
                 hexcount++;
                 instr = 0xf000;
                 instr = instr | (subpr >> 8);//instr = instr | (subpr >> 8);
                // myfile << "CALL subpr : " << dec_to_hex(instr) << "  2sw" << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = R1 & 0xff;
                 instr = instr | BTFSS;//BTFSS bits2
                 instr = instr | (bits2 << 9);
               //  instr = instr | R1;
               
                 //myfile << "BTFSS bits2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = BCF;//BCF S2
                     //int BitS1, adr, bitloc;
                 BitS1 = inpt.out[i];

                 adr = int(BitS1 - 1) / 8;
                 if (BitS1 % 8 != 0)
                     bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                 else bitloc = 7;
                 instr = instr | (bitloc << 9);
                 instr = instr | adr;
                 //myfile << "BCF S2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                 //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = R1 & 0xff;
                 instr = instr | BTFSC; //BTFCS bits2
                 instr = instr | (bits2 << 9);
                // instr = instr | R1;
                // myfile << "BTFCS bits2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 instr = BSF; // BSF S2
                 BitS1 = inpt.out[i];

                 adr = int(BitS1 - 1) / 8;
                 if (BitS1 % 8 != 0)
                     bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                 else bitloc = 7;
                 instr = instr | (bitloc << 9);
                 instr = instr | adr;
                // myfile << "BSF S2 : " << dec_to_hex(instr) << endl;
                 outf[hexcount] = dec_to_hex(instr);
                 hexcount++;
                //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                  //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 if (!inpt.stat[i])
                 {

                     if (inptport == 10)
                     {

                         BitS1 = StrToInt(oid);//inpt.intspec[i];//inpt.in[i];
                         adr = (int(BitS1 - 1) / 8) + valofst;
                         if (BitS1 % 8 != 0)
                             bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
                         else bitloc = 7;

                         instr = adr & 0xff;
                         instr = instr | BTG;
                         instr = instr | (bitloc << 9);
                         outf[hexcount] = dec_to_hex(instr);
                         hexcount++;
                     }
                     else
                     {
                         instr = Rinpt & 0xff;
                         instr = instr | BTG;
                         instr = instr | (inptport << 9);
                         // instr = instr | Rinpt;
                        //  myfile << "BTG i : " << dec_to_hex(instr) << endl;
                         outf[hexcount] = dec_to_hex(instr);
                         hexcount++;
                     }

                 }
                 ///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                 //------------------------------//

            

             //   myfile << dec_to_hex(instr) << endl;
             
             sig[inpt.out[i]] = 0;
        }

        
        //**************************

        i++;
      }
      i = 0;
      //processing the outputs..
      while (oupt.x[i] != NULL)
      {

           //myfile << (char)oupt.name[i][0]<< (char)oupt.name[i][1] << "  in : "<<oupt.in[i]<< endl;
          string oid = "";
          int inptport = 0;
          oid = oid + (char)oupt.name[i][0] + (char)oupt.name[i][1];
        //  myfile << "output : " <<oid<<" | "<< oupt.in[i]<< endl;

          if (oid == "A0")
              inptport = A0;
          else if (oid == "A1")
              inptport = A1;
          else if (oid == "A2")
              inptport = A2;
          else if (oid == "A3")
              inptport = A3;
          else if (oid == "A4")
              inptport = A4;
          else if (oid == "A5")
              inptport = A5;
          else if (oid == "A6")
              inptport = A6;
          else if (oid == "A7")
              inptport = A7;
          else if (oid[0] == 'M')
          {
              inptport = 10;
              int y = 1;
              while ((char)oupt.name[i][y] != ' ' && (char)oupt.name[i][y] != NULL)
              {
                  oid = oid + (char)oupt.name[i][y];
                  y++;
              }

              y = 0;
          }
          int instr = 0;

          int BitS1, adr, bitloc;
          //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          instr = BTFSS;//BTFSS S2
                 //int BitS1, adr, bitloc;
          BitS1 = oupt.in[i];

          adr = int(BitS1 - 1) / 8;
          if (BitS1 % 8 != 0)
              bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
          else bitloc = 7;
          instr = instr | (bitloc << 9);
          instr = instr | adr;
         
          // myfile << "BTFSS S2 : " << dec_to_hex(instr) << endl;
          outf[hexcount] = dec_to_hex(instr);
          hexcount++;
          //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if (inptport == 10)
          {
              instr = BCF;//BCF M
                     //int BitS1, adr, bitloc;
              BitS1 = StrToInt(oid);//inpt.out[i];

              adr = (int(BitS1 - 1) / 8 ) + valofst;
              if (BitS1 % 8 != 0)
                  bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
              else bitloc = 7;
              instr = instr | (bitloc << 9);
              instr = instr | adr;
              //myfile << "BCF S2 : " << dec_to_hex(instr) << endl;
              outf[hexcount] = dec_to_hex(instr);
              hexcount++;
          }
          else
          {
              instr = Routp & 0xff;
              instr = instr | BCF; // BCF q
             // instr = instr | Routp;
              instr = instr | (inptport << 9);
              // myfile << "BCF q : " << dec_to_hex(instr) << endl;
              outf[hexcount] = dec_to_hex(instr);
              hexcount++;
          }
          //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          instr = BTFSC;//BTFSC S2
                 //int BitS1, adr, bitloc;
          BitS1 = oupt.in[i];

          adr = int(BitS1 - 1) / 8;
          if (BitS1 % 8 != 0)
              bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
          else bitloc = 7;
          instr = instr | (bitloc << 9);
          instr = instr | adr;
         // myfile << "BTFSC S2 : " << dec_to_hex(instr) << endl;
          outf[hexcount] = dec_to_hex(instr);
          hexcount++;
         //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          if (inptport == 10)
          {
              instr = BSF; // BSF S2
              BitS1 = StrToInt(oid);//inpt.out[i];

              adr = (int(BitS1 - 1) / 8) + valofst;
              if (BitS1 % 8 != 0)
                  bitloc = (BitS1 - int(BitS1 / 8) * 8) - 1;
              else bitloc = 7;
              instr = instr | (bitloc << 9);
              instr = instr | adr;
              // myfile << "BSF S2 : " << dec_to_hex(instr) << endl;
              outf[hexcount] = dec_to_hex(instr);
              hexcount++;
          }
          else
          {
              //BSF q
              instr = Routp & 0xff;
              instr = instr | BSF; // BSF q
            //  instr = instr | Routp;
              instr = instr | (inptport << 9);
              //myfile << "BSF q : " << dec_to_hex(instr) << endl;
              outf[hexcount] = dec_to_hex(instr);
              hexcount++;
          }
          //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
          



          i++;
      }
    }
    i = 0;
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //MOVFF Routp,PORTB
    instr = MOVFF;
    instr = instr | Routp;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    instr = 0xF000;
    instr = instr | PORTB;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //GOTO PROG
    instr = prog & 0xff;
    instr = instr | GOTO;
    outf[hexcount] = dec_to_hex(instr);
    // myfile << "CALL subpr : " << dec_to_hex(instr) <<"  1sw" <<endl;
    hexcount++;
    instr = 0xf000;
    instr = instr | (prog >> 8);//instr = instr | (subpr >> 8);
   // myfile << "CALL subpr : " << dec_to_hex(instr) << "  2sw" << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

     //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                //GOTO start

    instr = start & 0xff;
    instr = instr | GOTO;
    outf[hexcount] = dec_to_hex(instr);
    // myfile << "CALL subpr : " << dec_to_hex(instr) <<"  1sw" <<endl;
    hexcount++;
    instr = 0xf000;
    instr = instr | (start >> 8);//instr = instr | (subpr >> 8);
   // myfile << "CALL subpr : " << dec_to_hex(instr) << "  2sw" << endl;
    outf[hexcount] = dec_to_hex(instr);
    hexcount++;
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    /*while (outf[i] != "")
    {

        myfile << outf[i] << "  ";
        i++;
    }
    */
   // myfile << endl<<endl<<endl;
    myfile << ":020000040000FA" << endl;
    i = 0;
    int bbb=0;
    int cheksum=0;
    int adrs = (0x7ffe - instrcount);// 0x4;
    string adress;
    string val = "",val1="";
    string str1, str2;
    string datacount;
    string data;
    while (outf[i] != "")
    {

       

        if (bbb == 8)
        {
            datacount = dec_to_hex(bbb*2);

            val = datacount[2];
            val = val + datacount[3];
            cheksum = hex_to_dec(val);

            data = dec_to_hex(adrs);

            val = data[2];
            val = val + data[3];
            cheksum = hex_to_dec(val)+ cheksum;

            val = data[0];
            val = val + data[1];
            cheksum = hex_to_dec(val) + cheksum;


            
            myfile << ":" << datacount[2] << datacount[3]<< dec_to_hex(adrs) <<"00";
            for (int count = 0; count < 8; count++)
            {
                str1 = outf[(i - 8) + count];
                str2 = str1[2];
                str2 = str2+str1[3];
                str2 = str2+str1[0];
                str2 = str2 + str1[1];

                myfile << str2;

                val = outf[(i - 8) + count][0];
                val = val + outf[(i - 8) + count][1];

                cheksum = hex_to_dec(val)+ cheksum;
                val = outf[(i - 8) + count][2];
                val = val + outf[(i - 8) + count][3];
                cheksum = hex_to_dec(val) + cheksum;

            }
            bbb = 0;

           
            cheksum ^= 0xfff;
            cheksum++;
            val = dec_to_hex(cheksum);
            val1 = val[2];
            val1 = val1 + val[3];
            myfile <<val1;
   
            myfile << endl;
            adrs = adrs + 16;
        }
        bbb++;
        i++;
    }

    i = i - bbb;
    datacount = dec_to_hex(bbb * 2);

    val = datacount[2];
    val = val + datacount[3];
    cheksum = hex_to_dec(val);

    data = dec_to_hex(adrs);

    val = data[2];
    val = val + data[3];
    cheksum = hex_to_dec(val)+ cheksum;

    val = data[0];
    val = val + data[1];
    cheksum = hex_to_dec(val) + cheksum;




    myfile << ":" << datacount[2] << datacount[3] << dec_to_hex(adrs) << "00";
    while (outf[i] != "")
    {    

        str1 = outf[i];
        str2 = str1[2];
        str2 = str2 + str1[3];
        str2 = str2 + str1[0];
        str2 = str2 + str1[1];

        myfile << str2;
          
            val = outf[i][0];
            val = val + outf[i][1];
            cheksum = hex_to_dec(val) + cheksum;
            val = outf[i][2];
            val = val + outf[i][3];
            cheksum = hex_to_dec(val) + cheksum;
       
        i++;

    }
    
    cheksum ^= 0xfff;
    cheksum++;
    val = dec_to_hex(cheksum);
    val1 = val[2];
    val1 = val1 + val[3];
    myfile << val1<<endl;

    myfile << ":00000001FF";

    int msgboxID = MessageBox(
        NULL,
        L"BUILD secess !!",
        L"BUILD ",
        MB_ICONASTERISK | MB_OK  //MB_ICONEXCLAMATION
    );

    
    myfile.close();
    return;
}

///**************************************************************///

///***********************************************************///
///                                                           ///
///           FUNCTION : dec_to_hex(int num4)                 ///
///             OUTPUT : convert DEC to hex                   ///
///                                                           ///
///***********************************************************///
string dec_to_hex(int num4)
{
    int num5[100];
    int ji;
    for (ji = 0; ji < 100; ji++)
    {
        num5[ji] = 0;
    }
    string str4 = "";

    if (num4 == 0)
    {
        return "0000";
    }
    else
    {
        for (ji = 0; num4 > 0; ji++)
        {
            num5[ji] = num4 % 16;
            num4 = num4 / 16;
        }

        for (ji = 3; ji >= 0; ji--)//for (j = 7; j >= 0; j--)
        {
            switch (num5[ji])
            {
            case 0:
                str4 = str4 + "0";
                break;
            case 1:
                str4 = str4 + "1";
                break;
            case 2:
                str4 = str4 + "2";
                break;
            case 3:
                str4 = str4 + "3";
                break;
            case 4:
                str4 = str4 + "4";
                break;
            case 5:
                str4 = str4 + "5";
                break;
            case 6:
                str4 = str4 + "6";
                break;
            case 7:
                str4 = str4 + "7";
                break;
            case 8:
                str4 = str4 + "8";
                break;
            case 9:
                str4 = str4 + "9";
                break;
            case 10:
                str4 = str4 + "A";
                break;
            case 11:
                str4 = str4 + "B";
                break;
            case 12:
                str4 = str4 + "C";
                break;
            case 13:
                str4 = str4 + "D";
                break;
            case 14:
                str4 = str4 + "E";
                break;
            case 15:
                str4 = str4 + "F";
                break;
            default:
                str4 = str4 + "0";
                break;
            }
        }
        return str4;
    }

}
///***********************************************************///
///***********************************************************///
///                                                           ///
///            FUNCTION : hex_to_dec(string str1)             ///
///                   convert HEX value to DEC                ///
///                                                           ///
///***********************************************************///
int hex_to_dec(string str1)
{
    string x;
    int num0[100];
    int num11 = 0;
    int j11=0, j33;
    int ji = 0;
    while (str1[ji] != NULL)
    {
        switch (str1[ji])
        {
        case 'A':
            num0[ji] = 10;
            break;
        case 'B':
            num0[ji] = 11;
            break;
        case 'C':
            num0[ji] = 12;
            break;
        case 'D':
            num0[ji] = 13;
            break;
        case 'E':
            num0[ji] = 14;
            break;
        case 'F':
            num0[ji] = 15;
            break;
        case '0':
            num0[ji] = 0;
            break;
        case '1':
            num0[ji] = 1;
            break;
        case '2':
            num0[ji] = 2;
            break;
        case '3':
            num0[ji] = 3;
            break;
        case '4':
            num0[ji] = 4;
            break;
        case '5':
            num0[ji] = 5;
            break;
        case '6':
            num0[ji] = 6;
            break;
        case '7':
            num0[ji] = 7;
            break;
        case '8':
            num0[ji] = 8;
            break;
        case '9':
            num0[ji] = 9;
            break;

        default:
            x = str1[ji];
           // num0[j] = stoi(x);
            break;
        }
        j11 = ji;
        ji++;
    }
    ji = 0;
    j33 = 0;
    for (int j22 = j11; j22 >= 0; j22--)
    {
        num11 = num11 + (num0[j22] * pow(16, j33));
        j33++;
    }
    j33 = 0;

    return num11;

}

///***********************************************************///

///***********************************************************************///
///                                                                       ///
///  FUNCTION : InstrProcess(int var1 , int var2 , int var3 , int inst)   ///
///                 OUTPUT: process the instructiones                     ///    
///                                                                       ///
///***********************************************************************///

string InstrProcess(int var1 , int var2 , int var3 , int inst)
{
    int instruc;
    string hexcode;
    switch (inst)
    {
        case CLRF :
        {           
            instruc = var1 & var2;
            instruc = instruc | CLRF;
            hexcode = dec_to_hex(instruc);
            return hexcode;
        }
        break;


    }


}
///***********************************************************************///

///***********************************************************************///
///                                                                       ///
///               FUNCTION :  StrToInt(string)                            ///
///               OUTPUP : convert string to int                          ///
///                                                                       ///
///***********************************************************************///
int  StrToInt(string str)
{
    int valo = 0;
    int k = 0;
    int k1 = 1;
    while (str[k] != NULL)
    {
        k++;
    }
    k--;
    while (k >= 0)
    {
        switch (str[k])
        {
        case '0':
            valo = valo + (0 * k1);
            break;
        case '1':
            valo = valo + (1 * k1);
            break;
        case '2':
            valo = valo + (2 * k1);
            break;
        case '3':
            valo = valo + (3 * k1);
            break;
        case '4':
            valo = valo + (4 * k1);
            break;
        case '5':
            valo = valo + (5 * k1);
            break;
        case '6':
            valo = valo + (6 * k1);
            break;
        case '7':
            valo = valo + (7 * k1);
            break;
        case '8':
            valo = valo + (8 * k1);
            break;
        case '9':
            valo = valo + (9 * k1);
            break;

        }

        k--;
        k1 = k1 * 10;
    }
    return valo;
}
///***********************************************************************///

///****************************************************************************///
///           FUNCTION :  void sendprog()                                      ///
///   OUTPUT : send program to the PIC18F2550 microcontroller to be programed  /// 
///                                                                            ///
///****************************************************************************///
void sendprog()
{
    WCHAR uoo[1];

    DWORD nNumberOfBytesToRead = 1;
    DWORD lpNumberOfBytesRead = 0;

    //****
    fstream myfile;
    myfile.open("tes.hex");

    int i = 0;
    int j = 0;
    int bytecont = 0;
    int adroffst = 0;
    bool st = true;
    int soul;
    string a;
    string val = "";
    string val1 = "";
    char b[500];

    char bb[38];



    if (myfile.is_open())
    {

        for (i = 0; i < 500; i++)
        {
            b[i] = 0xff;
        } 
       // b[0] = 'P';
       // b[1] = 'R';
      // b[2] = 'O';
       // b[3] = 'G';
        a = "aa";
        myfile >> a;
        a[0] = '0';
        j = 0;
       

        while (a[0] != 'a')
        {
            a = "aa";
            myfile >> a;
            if (st)
            {
                string valad = "";
                valad = a[3];
                valad = valad + a[4];
                valad = valad + a[5];
                valad = valad + a[6];


                adroffst = hex_to_dec(valad);
                st = false;
            }
            val = a[0];

            val = a[1];
            val = val + a[2];
            soul = hex_to_dec(val);


            for (i = 0; i < soul * 2; i = i + 4)
            {
                val = a[i + 9];
                val = val + a[i + 10];

                val1 = a[i + 11];
                val1 = val1 + a[i + 12];

                b[j] = hex_to_dec(val);
                b[(j + 1)] = hex_to_dec(val1);

                j = j + 2;
               // bytecont = bytecont + 2;
            }

        }
        j = 0;
  
    }
    else {
        int msgboxID = MessageBox(
            NULL,
            L"Can't open tes.hex file",
            L"can't open ",
            MB_ICONSTOP | MB_OK //MB_ICONEXCLAMATION
        );
        return;
    }

    myfile.close();
   // HANDLE hComm;

    hComm = CreateFile(L"\\\\.\\COM14",                //port name   L"\\\\.\\COM5"
        GENERIC_READ | GENERIC_WRITE, //Read/Write
        0,                            // No Sharing
        NULL,                         // No Security
        OPEN_EXISTING,// Open existing port only
        0,            // Non Overlapped I/O
        NULL);        // Null for Comm Devices

    if (hComm == INVALID_HANDLE_VALUE)
    {
       
         MessageBox(
            NULL,
            L"Error in opening serial port",
            L"Confirm Save As",
            MB_ICONSTOP  | MB_OK //MB_ICONEXCLAMATION
        );
        return;

    }
    else
    {       
        
    }
   char lpBuffer[] = "OF";
    DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
    DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
    

    char fuf[] = "ERES";
   
    dNoOFBytestoWrite = 4;
    WriteFile(hComm,        // Handle to the Serial port
        fuf,// lpBuffer,     // Data to be written to the port
        dNoOFBytestoWrite,  //No of bytes to write
        &dNoOfBytesWritten, //Bytes written
        NULL); 
   
    //****************************************
    int numbbit = 0;
    string ofss="";
    string ofss1 = "";
    string ofss2 = "";
    
    //numbbit = 0x1f - ( adroffst & 0x1f) + 1 ;
    int bund;
    bund = adroffst & 0x1f;
    bb[0] = 'P';
    bb[1] = 'R';
    bb[2] = 'O';
    bb[3] = 'G';
    ofss = dec_to_hex((adroffst & 0xffe0));
    ofss1 = ofss[0];
    ofss1 = ofss1 + ofss[1];
    ofss2 = ofss[2];
    ofss2 = ofss2 + ofss[3];
   
    bb[4] = hex_to_dec(ofss1);
    bb[5] = hex_to_dec(ofss2);
    

    j = 0;
    adroffst = adroffst & 0xffe0; 
    while ( adroffst < 0x8000)
    {

        
        for (i = 6; i < 38; i++)
        {
            if ( i < bund+6)
            {
                bb[i] = 0xff;
            }
            else
            {
                bb[i] = b[j];
                j++;
            }
            
        }
        dNoOFBytestoWrite = 38;
        dNoOfBytesWritten = 0;

        WriteFile(hComm,        // Handle to the Serial port
            bb,// lpBuffer,     // Data to be written to the port
            dNoOFBytestoWrite,  //No of bytes to write
            &dNoOfBytesWritten, //Bytes written
            NULL);
        adroffst += 0x20;
        bund = adroffst & 0x1f;
        ofss = dec_to_hex((adroffst & 0xffe0));
        ofss1 = ofss[0];
        ofss1 = ofss1 + ofss[1];
        ofss2 = ofss[2];
        ofss2 = ofss2 + ofss[3];

        bb[4] = hex_to_dec(ofss1);
        bb[5] = hex_to_dec(ofss2);

    }


   dNoOfBytesWritten = 0;
    dNoOFBytestoWrite = sizeof(lpBuffer);
    WriteFile(hComm,        // Handle to the Serial port
        lpBuffer,     // Data to be written to the port
        dNoOFBytestoWrite,  //No of bytes to write
        &dNoOfBytesWritten, //Bytes written
        NULL);  



   CloseHandle(hComm); //;//Closing the Serial Port
     
         MessageBox(
             NULL,
             L"DONE PROGRAMING",
             L"Confirm Save As",
             MB_ICONEXCLAMATION | MB_OK
         );    
    
}
///****************************************************************************///

