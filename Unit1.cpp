//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <iostream.h>
#include <math.h>
#include <stdio.h>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
   Edit1->Text = " ";
   Memo1->Lines->Clear();
}

struct Tree
{
   int info;
   Tree *left, *right;
}*root;		      // ������

void Add_List(Tree*, int);
void View_Tree (Tree*, int);
Tree* Del_Info(Tree*, int);
void Del_Tree(Tree*);
Tree* List(int);
void Make_Blns(Tree**, int, int, int*);
Tree* Min_Key(Tree*);


//---------------------------�����������------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
   if( root == NULL ) ShowMessage(" �������� ������!");
   else
   {
      Memo1->Lines->Add("---------- ������ -----------");
      View_Tree(root, 0);
   }
}
//------------------------��������---------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
   if(Edit1->Text == " ") ShowMessage(" ������� �������!");
   if(root == NULL) root = List (StrToInt(Edit1->Text));
   else Add_List (root, StrToInt(Edit1->Text));
}
//------------------------������� ����---------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
   int b = StrToInt(Form1->Edit1->Text);
   root = Del_Info(root, b);
}
//--------------------���������� ������-------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
   Del_Tree(root);
   ShowMessage(" ������ �������!");
   root = NULL;
}
//----------------------EXIT-----------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender)
{
   if(root!=NULL)
   {
      Del_Tree(root);
      ShowMessage(" ������ �������!");
   }
   Close();
}


//---------------������������ ������ ������------------------------------------------------------------
Tree* List(int inf)
{
   Tree *t = new Tree;		// ������ ������
   t -> info = inf;			// ������������ �������������� �����
   t -> left = t -> right = NULL;	// ������������ �������� ������
   return t;				// ������� ���������� ���������
}
//------------------------------------------
void Add_List(Tree *root, int key)
{
   Tree *prev, *t;     // prev - ��������� ������ ������ �����
   bool find = true;
   t = root;
   while ( t && find) {
      prev = t;
      if( key == t->info)
      {
         find = false;	 	// ���� ������ ���� ��������
         ShowMessage("����� ������� ��� ����!");
      }
      else
         if ( key < t -> info ) t = t -> left;
         else   t = t -> right;
   }
   if (find)
   {		// ����� ������ �����
      t = List(key);			// ������� ����� ����
      if ( key < prev -> info ) prev -> left = t;
      else    prev -> right = t;
   }
}
//---------------------������� ��������� ��������� ������---------
void View_Tree(Tree *p, int level)
{
   String str;
   if ( p )
   {
      View_Tree (p -> right , level+1);		// ������ ���������
      for ( int i=0; i<level; i++) str = str + "    ";
      Form1->Memo1->Lines->Add(str + IntToStr(p->info));
      View_Tree(p -> left , level+1); 			// ����� ���������
   }
}
//---------------------��������-------------------------
Tree* Del_Info(Tree *root, int key)
{
   Tree *Del, *Prev_Del, *R, *Prev_R;
   // Del, Prev_Del - ��������� ���� � ��� ���������� (������);
   // R, Prev_R - �������, �� ������� ���������� ���������, � ��� ������;
   // R, Prev_R - �������, �� ������� ���������� ���������, � ��� ������;
   Del = root;
   Prev_Del = NULL;
   //-------- ����� ���������� �������� � ��� ������ �� ����� key ---------
   while (Del != NULL && Del -> info != key) {
      Prev_Del = Del;
      if (Del->info > key)  Del = Del->left;
      else Del = Del->right;
   }
   if (Del == NULL)
   {				// ������� �� ������
      ShowMessage ( "������ �������� ���!");
      return root;
   }
   //-------------------- ����� �������� R ��� ������ --------------------------------
   if (Del -> right == NULL) R = Del->left;
   else
      if (Del -> left == NULL) R = Del->right;
      else
      { //------------ ���� ����� ������ ���� � ����� ��������� -----------------
         Prev_R = Del;
         R = Del->left;
         while (R->right != NULL) {
            Prev_R = R;
            R = R->right;
         }
         //----------- ����� ������� ��� ������ R � ��� ������ Prev_R -------------
         if( Prev_R == Del)  R->right = Del->right;
         else
         {
            R->right = Del->right;
            Prev_R->right = R->left;
            R->left = Prev_R;
         }
      }
      if (Del== root) root = R;			// ������ ������, �������� ��� �� R
      else
      //------- ��������� R ������������ � ������ ���������� ���� -----------
         if (Del->info < Prev_Del->info)
            Prev_Del->left = R;				// �� ����� �����
         else	Prev_Del->right = R;				// �� ������ �����
      delete Del;
      return root;
}
//-----------------����� ���� � ����������� (������������) ������:
Tree* Min_Key(Tree *p)
{			// Tree* Max_Key(Tree *p)
   while (p->left != NULL) p = p->left;	// p=p->right;
   Form1->Memo1->Lines->Add("������� = "+IntToStr(p->info));
   return p;
}
//--------------���������� ����������������� ������ ������
void Make_Blns(Tree **p, int n, int k, int *a)
{
   if (n == k)
   {
      *p = NULL;
      return;
   }
   else
   {
      int m=(n+k)/2;
      *p = new Tree;
      (*p)->info = a[m];
      Make_Blns( &(*p)->left, n, m, a);
      Make_Blns( &(*p)->right, m+1, k, a);
   }
}
//--------------������� ������������ ������, ������� �������
void Del_Tree(Tree *t)
{
   if ( t != NULL)
   {
      Del_Tree ( t -> left); 			// �� ����� �����
      Del_Tree ( t -> right); 			// �� ������ �����
      delete t;
   }
}

//-----------------------���������� ������������----------------------------------------------------
void __fastcall TForm1::Button7Click(TObject *Sender)
{
   if( root == NULL ) ShowMessage(" �������� ������!");
   else
   {
      Min_Key(root);
   }
}
//---------------------------------------------------------------------------


