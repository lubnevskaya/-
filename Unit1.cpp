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
}*root;		      // Корень

void Add_List(Tree*, int);
void View_Tree (Tree*, int);
Tree* Del_Info(Tree*, int);
void Del_Tree(Tree*);
Tree* List(int);
void Make_Blns(Tree**, int, int, int*);
Tree* Min_Key(Tree*);


//---------------------------Просмотреть------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
   if( root == NULL ) ShowMessage(" Создайте дерево!");
   else
   {
      Memo1->Lines->Add("---------- Дерево -----------");
      View_Tree(root, 0);
   }
}
//------------------------Добавить---------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
   if(Edit1->Text == " ") ShowMessage(" Введите элемент!");
   if(root == NULL) root = List (StrToInt(Edit1->Text));
   else Add_List (root, StrToInt(Edit1->Text));
}
//------------------------Удалить инфо---------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
   int b = StrToInt(Form1->Edit1->Text);
   root = Del_Info(root, b);
}
//--------------------Освободить память-------------------------------------------------------
void __fastcall TForm1::Button5Click(TObject *Sender)
{
   Del_Tree(root);
   ShowMessage(" Дерево удалено!");
   root = NULL;
}
//----------------------EXIT-----------------------------------------------------
void __fastcall TForm1::Button6Click(TObject *Sender)
{
   if(root!=NULL)
   {
      Del_Tree(root);
      ShowMessage(" Дерево удалено!");
   }
   Close();
}


//---------------Формирование дерева поиска------------------------------------------------------------
Tree* List(int inf)
{
   Tree *t = new Tree;		// Захват памяти
   t -> info = inf;			// Формирование информационной части
   t -> left = t -> right = NULL;	// Формирование адресных частей
   return t;				// Возврат созданного указателя
}
//------------------------------------------
void Add_List(Tree *root, int key)
{
   Tree *prev, *t;     // prev - указатель предка нового листа
   bool find = true;
   t = root;
   while ( t && find) {
      prev = t;
      if( key == t->info)
      {
         find = false;	 	// Ключ должен быть уникален
         ShowMessage("Такой элемент уже есть!");
      }
      else
         if ( key < t -> info ) t = t -> left;
         else   t = t -> right;
   }
   if (find)
   {		// Нашли нужное место
      t = List(key);			// Создаем новый лист
      if ( key < prev -> info ) prev -> left = t;
      else    prev -> right = t;
   }
}
//---------------------Функция просмотра элементов дерева---------
void View_Tree(Tree *p, int level)
{
   String str;
   if ( p )
   {
      View_Tree (p -> right , level+1);		// Правое поддерево
      for ( int i=0; i<level; i++) str = str + "    ";
      Form1->Memo1->Lines->Add(str + IntToStr(p->info));
      View_Tree(p -> left , level+1); 			// Левое поддерево
   }
}
//---------------------Удаление-------------------------
Tree* Del_Info(Tree *root, int key)
{
   Tree *Del, *Prev_Del, *R, *Prev_R;
   // Del, Prev_Del - удаляемый узел и его предыдущий (предок);
   // R, Prev_R - элемент, на который заменяется удаленный, и его предок;
   // R, Prev_R - элемент, на который заменяется удаленный, и его предок;
   Del = root;
   Prev_Del = NULL;
   //-------- Поиск удаляемого элемента и его предка по ключу key ---------
   while (Del != NULL && Del -> info != key) {
      Prev_Del = Del;
      if (Del->info > key)  Del = Del->left;
      else Del = Del->right;
   }
   if (Del == NULL)
   {				// Элемент не найден
      ShowMessage ( "Такого элемента нет!");
      return root;
   }
   //-------------------- Поиск элемента R для замены --------------------------------
   if (Del -> right == NULL) R = Del->left;
   else
      if (Del -> left == NULL) R = Del->right;
      else
      { //------------ Ищем самый правый узел в левом поддереве -----------------
         Prev_R = Del;
         R = Del->left;
         while (R->right != NULL) {
            Prev_R = R;
            R = R->right;
         }
         //----------- Нашли элемент для замены R и его предка Prev_R -------------
         if( Prev_R == Del)  R->right = Del->right;
         else
         {
            R->right = Del->right;
            Prev_R->right = R->left;
            R->left = Prev_R;
         }
      }
      if (Del== root) root = R;			// Удаляя корень, заменяем его на R
      else
      //------- Поддерево R присоединяем к предку удаляемого узла -----------
         if (Del->info < Prev_Del->info)
            Prev_Del->left = R;				// На левую ветвь
         else	Prev_Del->right = R;				// На правую ветвь
      delete Del;
      return root;
}
//-----------------Поиск узла с минимальным (максимальным) ключом:
Tree* Min_Key(Tree *p)
{			// Tree* Max_Key(Tree *p)
   while (p->left != NULL) p = p->left;	// p=p->right;
   Form1->Memo1->Lines->Add("Минимум = "+IntToStr(p->info));
   return p;
}
//--------------Построение сбалансированного дерева поиска
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
//--------------Функция освобождения памяти, занятой деревом
void Del_Tree(Tree *t)
{
   if ( t != NULL)
   {
      Del_Tree ( t -> left); 			// На левую ветвь
      Del_Tree ( t -> right); 			// На правую ветвь
      delete t;
   }
}

//-----------------------Нахождение минимального----------------------------------------------------
void __fastcall TForm1::Button7Click(TObject *Sender)
{
   if( root == NULL ) ShowMessage(" Создайте дерево!");
   else
   {
      Min_Key(root);
   }
}
//---------------------------------------------------------------------------


