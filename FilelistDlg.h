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
	 void startsys();  // �����ļ�ϵͳ  
	 void my_format();  // ���̸�ʽ��  
	 void my_cd(char *dirname);  // ���ĵ�ǰĿ¼  
	 void my_mkdir(char *dirname);  // ������Ŀ¼  
	 void my_rmdir(char *dirname);  // ɾ����Ŀ¼  
	 void my_ls();  // ��ʾĿ¼  
	 void my_create (char *filename);  // �����ļ�  
	 void my_rm(char *filename);  // ɾ���ļ�  
	 int my_open(char *filename);  // ���ļ�  
	 int my_close(int fd);  // �ر��ļ�  
	 int my_write(int fd);  // д�ļ�  
	 int do_write(int fd, char *text, int len, char wstyle);  // ʵ��д�ļ�  
	 int my_read(int fd, int len);  // ���ļ�  
	 int do_read(int fd, int len,char *text);// ʵ�ʶ��ļ�  
	 void my_exitsys();  // �˳��ļ�ϵͳ  
     unsigned short findblock();  // Ѱ�ҿ����̿�  
	 int findopenfile();  // Ѱ�ҿ����ļ�����  

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
