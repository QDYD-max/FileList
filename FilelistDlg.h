#if !defined(AFX_FILELISTDLG_H__ED5C1E82_74D1_406A_8540_DD8CB9095A78__INCLUDED_)
#define AFX_FILELISTDLG_H__ED5C1E82_74D1_406A_8540_DD8CB9095A78__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilelistDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilelistDlg dialog

class CFilelistDlg : public CDialog
{
// Construction
public:
	CFilelistDlg(CWnd* pParent = NULL);   // standard constructor
	 void startsys();  // 进入文件系统  
	 void my_format();  // 磁盘格式化  
	 void my_cd(char *dirname);  // 更改当前目录  
	 void my_mkdir(char *dirname);  // 创建子目录  
	 void my_rmdir(char *dirname);  // 删除子目录  
	 void my_ls();  // 显示目录  
	 void my_create (char *filename);  // 创建文件  
	 void my_rm(char *filename);  // 删除文件  
	 int my_open(char *filename);  // 打开文件  
	 int my_close(int fd);  // 关闭文件  
	 int my_write(int fd);  // 写文件  
	 int do_write(int fd, char *text, int len, char wstyle);  // 实际写文件  
	 int my_read(int fd, int len);  // 读文件  
	 int do_read(int fd, int len,char *text);// 实际读文件  
	 void my_exitsys();  // 退出文件系统  
     unsigned short findblock();  // 寻找空闲盘块  
	 int findopenfile();  // 寻找空闲文件表项  

	 CImageList m_ImageList;
// Dialog Data
	//{{AFX_DATA(CFilelistDlg)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_local_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilelistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg LRESULT createFile(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT createFolder(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT saveFile(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT closeFile(WPARAM wparam,LPARAM lparam);
	// Generated message map functions
	//{{AFX_MSG(CFilelistDlg)
	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRopen(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdelete();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILELISTDLG_H__ED5C1E82_74D1_406A_8540_DD8CB9095A78__INCLUDED_)
