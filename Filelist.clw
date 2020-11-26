; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFilelistDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Filelist.h"
LastPage=0

ClassCount=12
Class1=CFilelistApp
Class2=CFilelistDoc
Class3=CFilelistView
Class4=CMainFrame

ResourceCount=9
Resource1=IDR_MENU2
Resource2=IDD_DIALOG1
Resource3=IDR_FILELITYPE
Class5=CChildFrame
Class6=CAboutDlg
Class7=CFilelistDlg
Resource4=IDD_DIALOG3
Resource5=IDD_ABOUTBOX
Resource6=IDR_MENU1
Resource7=IDR_MAINFRAME
Class8=CFileDlg
Class9=CFolderCreate
Class10=CFolderDlg
Class11=CFileCreate
Resource8=IDD_DIALOG2
Class12=CFileOpen
Resource9=IDD_DIALOG4

[CLS:CFilelistApp]
Type=0
HeaderFile=Filelist.h
ImplementationFile=Filelist.cpp
Filter=N

[CLS:CFilelistDoc]
Type=0
HeaderFile=FilelistDoc.h
ImplementationFile=FilelistDoc.cpp
Filter=N

[CLS:CFilelistView]
Type=0
HeaderFile=FilelistView.h
ImplementationFile=FilelistView.cpp
Filter=C
LastObject=IDM_DIALOG
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=Filelist.cpp
ImplementationFile=Filelist.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_FILELITYPE]
Type=1
Class=CFilelistView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
Command22=IDM_DIALOG
CommandCount=22

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG1]
Type=1
Class=CFilelistDlg
ControlCount=1
Control1=IDC_LIST1,SysListView32,1350631425

[CLS:CFilelistDlg]
Type=0
HeaderFile=FilelistDlg.h
ImplementationFile=FilelistDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_ROPEN

[MNU:IDR_MENU1]
Type=1
Class=CFileDlg
Command1=IDM_MYDIR
Command2=ID_MYFILE
CommandCount=2

[MNU:IDR_MENU2]
Type=1
Class=CFilelistDlg
Command1=ID_ROPEN
Command2=ID_RDELETE
CommandCount=2

[DLG:IDD_DIALOG2]
Type=1
Class=CFileDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG3]
Type=1
Class=CFolderDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_EDIT2,edit,1350631552
Control3=IDC_STATIC,static,1342308352

[CLS:CFileDlg]
Type=0
HeaderFile=FileDlg.h
ImplementationFile=FileDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_MYFILE
VirtualFilter=dWC

[CLS:CFolderCreate]
Type=0
HeaderFile=FolderCreate.h
ImplementationFile=FolderCreate.cpp
BaseClass=CFileDialog
Filter=D
VirtualFilter=dWC

[CLS:CFolderDlg]
Type=0
HeaderFile=FolderDlg.h
ImplementationFile=FolderDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDM_MYDIR
VirtualFilter=dWC

[CLS:CFileCreate]
Type=0
HeaderFile=FileCreate.h
ImplementationFile=FileCreate.cpp
BaseClass=CButton
Filter=W

[DLG:IDD_DIALOG4]
Type=1
Class=CFileOpen
ControlCount=2
Control1=IDC_EDIT1,edit,1352732804
Control2=IDC_BUTTON1,button,1342242816

[CLS:CFileOpen]
Type=0
HeaderFile=FileOpen.h
ImplementationFile=FileOpen.cpp
BaseClass=CDialog
Filter=D
LastObject=CFileOpen
VirtualFilter=dWC

