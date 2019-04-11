//#include "stdafx.h"
//#include <iostream>
#include <stdio.h>
using namespace std; 

class BSTNode
{
public:
   int NodeKey; 
   int NodeValue; 
   BSTNode* Parent; // parent node or NULL for the Root
   BSTNode* LeftChild; 
   BSTNode* RightChild; 

   BSTNode(int key, int val, BSTNode* parent)
   {
      NodeKey = key;
      NodeValue = val;
      Parent = parent;
      LeftChild = NULL;
      RightChild = NULL;
   }
};

//============= not my code ==============
struct QueueNode
{
    BSTNode* trans;
    QueueNode* next;
};
 
class Queue
{
    public:
        QueueNode *Head;
    
        Queue()
        {
            Head = NULL;
        }
 
        bool isEmpty()
        {
            if(Head == NULL)
                return true;
            else 
                return false;
        }
 
        void addToQueue(BSTNode *tmp)
        { 
            QueueNode *Ptr = new QueueNode;
            Ptr -> trans = tmp;
 
            if(Head == NULL)
            {
                Head = Ptr;
                Ptr -> next = NULL; 
            }
            else
            {
                QueueNode *tmp_c = Head;
 
                while(tmp_c -> next != NULL)
                    tmp_c = tmp_c -> next;
 
                tmp_c -> next = Ptr;
                Ptr -> next = NULL; 
            }
        }

        void delQueue()
        {
           if(Head == NULL) 
              return;
            QueueNode *node = Head -> next;
             
            while(Head->next != NULL)
            {                
               delete Head;
               Head = node;
               node = Head -> next;                                
            }
            delete Head;
            Head = NULL;
        }
 
        BSTNode* getHead()
        {
            if(Head == NULL) 
              return NULL;
            BSTNode* node = Head->trans;
            QueueNode* Qnode = Head;
            Head = Head->next;            
            delete Qnode;
            return node;
        }

        //void print()
        //{
        //    QueueNode *node = Head;
        //    while (node != NULL)
        //    {
        //       cout << node->trans->NodeKey << "  ";
        //       node = node->next;
        //    }
        //    cout << endl;
        //}
};
//Queue list;
//========================================

class BSTFind   // intermediate search result
{
public:
   BSTNode* Node;       // the last node in the serch (may be Root)
   bool NodeHasKey;     // true if the key was found 
   bool ToLeft;         // true, if child must be added to the left

   BSTFind() { Node = NULL; } // search result
};
BSTFind info;

class BST
{
public:
   BSTNode* Root; // tree Root or NULL  (How it can be NULL?)
   BSTNode* List [100];
   int countArr;

   BST(BSTNode* node)
   {
      Root = node;
      countArr = 0;
      for (int i = 0; i<100; i++)
         List [i] = NULL;
      
   }

   BSTFind* FindNodeByKey(int key)   // Search the node in the tree, as well as information by the key 
   {
      BSTFind* _info = &info;
      BSTNode* node = find_key(Root, key); // find key or NULL
      
      _info->Node = node;
      if (node->NodeKey == key)        
         _info->NodeHasKey = true;
      else
      {        
         _info->NodeHasKey = false;
         if (key < node->NodeKey)         
            _info->ToLeft = true;
         else
            _info->ToLeft = false;
      }      
      return _info;
   }

   bool AddKeyValue(int key, int val)   // add key-value to the tree
   {
      if (Root == NULL)
      {
         BSTNode* node = new BSTNode(key, val, NULL); 
         Root = node;
         return true;
      }
      BSTFind* _info = FindNodeByKey(key);
      if (_info->NodeHasKey)      
         return false; // if the key is already there     
      else
      {
         BSTNode* node = new BSTNode(key, val, _info->Node); 
         if (_info->ToLeft)
            _info->Node->LeftChild = node;
         else
            _info->Node->RightChild = node;
         return true;
      }     
   }

   BSTNode* FinMinMax(BSTNode* FromNode, bool FindMax)   // search MAX/MIN by the tree
   {      
      if (FindMax) {
         if (FromNode->RightChild != NULL)
            return FinMinMax(FromNode->RightChild, FindMax); } // MAX
      else {
         if (FromNode->LeftChild != NULL)
            return FinMinMax(FromNode->LeftChild, FindMax); } // MIN
      return FromNode;
   }

   bool DeleteNodeByKey(int key)
   {
      BSTFind* _info = FindNodeByKey(key);

      if (_info->NodeHasKey) // the key was found
      {       

         if (_info->Node->RightChild != NULL)  // remove with transfer of key information     
         {
            BSTNode* replacement = FinMinMax(_info->Node->RightChild, false);  // find replacement for node                 
            _info->Node->NodeKey = replacement->NodeKey;                       // replace key
            _info->Node->NodeValue = replacement->NodeValue;                   // replace value

            link_the_tail(replacement, false); 
            delete replacement; 
         }
         else // only delete the key node 
         {                               
            link_the_tail(_info->Node, true); // left child to left 
            delete _info->Node;
         }
         return true;
      }
      else
         return false; // if the node was not found
   }

   int Count()
   { 
      if (Root != NULL)
         return keys_count(Root); // number of nodes in the tree
      return 0;
   } 

   BSTNode** DeepAllNodes(int method)
   {       
      //list.delQueue();   // опусташаем список перед заполнением
      resetList();
      if (method == 0)
         in_order(Root);   // —имметричный или поперечный - обрабатываетс€ лева€ ветка, корень, права€ ветка
      if (method == 1)
         post_order(Root); // обратный обход - гооt обрабатываетс€ в конце
      if (method == 2)
         pre_order(Root);  // пр€мой обход - гооt обрабатываетс€ первым
      //return &list;
      return List;
   }
   
   BSTNode** WideAllNodes()
   {
       //list.delQueue();     // опусташаем список перед заполнением 
       resetList();
       Queue *q = new Queue();        //ƒл€ начала поместим в очередь корень    
       q->addToQueue(Root); 
       int i = 0;
       while ( !q->isEmpty() ) {
           BSTNode* tmp = q->getHead();
           List[countArr++] = tmp;
           //list.addToQueue(tmp);        
           if (tmp->LeftChild) {                 //≈сли есть левый наследник, то помещаем его в очередь дл€ дальнейшей обработки
               q->addToQueue(tmp->LeftChild);
           }         
           if (tmp->RightChild) {                //≈сли есть правый наследник, то помещаем его в очередь дл€ дальнейшей обработки
               q->addToQueue(tmp->RightChild);
           }
       }
       q->delQueue();
       delete q;
       return List;
   }
 
   void deleteTree(BSTNode** root) {
       if (*root) {
           deleteTree( &((*root)->LeftChild) );
           deleteTree( &((*root)->RightChild) );
           delete(*root);
       }
   }

private:
   void resetList()
   {
      countArr = 0;
      for (int i = 0; i<100; i++)
         List [i] = NULL;
   }

   void in_order(BSTNode* node)
   {        
      if (node == NULL)
         return ;
      in_order(node->LeftChild);
      List[countArr++] = node;
      //list.addToQueue(node);
      in_order(node->RightChild);      
      return ;    
   }

   void post_order(BSTNode* node)
   {  
      if (node == NULL)
         return ;     
      post_order(node->LeftChild);
      post_order(node->RightChild); 
      List[countArr++] = node;
      //list.addToQueue(node);
      return ;    
   }

   void pre_order(BSTNode* node)
   {  
      if (node == NULL)
         return ;  
      //list.addToQueue(node);
      List[countArr++] = node;
      pre_order(node->LeftChild);
      pre_order(node->RightChild);       
      return ;    
   }

   void link_the_tail(BSTNode* parent, bool left_child)
   {
      if(left_child)
      {
         if (parent != Root)
         {
            if (parent->LeftChild != NULL)                     // if child is not NULL
               parent->LeftChild->Parent = parent->Parent;     // link child to new parent
            if (parent->Parent->LeftChild == parent)           // what parent link to update?
               parent->Parent->LeftChild = parent->LeftChild;  // update the left parent link (new child or NULL) 
            else 
               parent->Parent->RightChild = parent->LeftChild; // update the right parent link (new child or NULL) 
         }
         else 
         {
            Root = parent->LeftChild;
            if (Root != NULL)
               Root->Parent = NULL;
         }
      }
      else // if RightChild
      {
         if (parent->RightChild != NULL) 
            parent->RightChild->Parent = parent->Parent;     

         if (parent->Parent->LeftChild == parent)
            parent->Parent->LeftChild = parent->RightChild;   
         else 
            parent->Parent->RightChild = parent->RightChild; 
      }
   }
   
   BSTNode* find_key(BSTNode* node, int key) // search 
   {
         if (key < node->NodeKey) {
            if (node->LeftChild != NULL)
              return find_key(node->LeftChild, key); }
         else {
            if (key > node->NodeKey)
               if (node->RightChild != NULL)
                 return find_key(node->RightChild, key); }
         return node;
   } 

   int keys_count(BSTNode* node)
   {      
      int s = 0; 
      if (node == NULL)
         return 0;        
      s += keys_count(node->LeftChild);
      s += keys_count(node->RightChild);
      s++;
      return s;    
   }
};


int test()
{
   int test = 0;
   BSTNode* node;
   BSTFind* fainded_node_info;
   BSTFind* not_fainded_node_info;
   BST tree(new BSTNode(8, 0, NULL));   // create the tree

   // filling the tree
   if (!tree.AddKeyValue(4, 1)) test++;
   if (!tree.AddKeyValue(2, 2)) test++;
   if (!tree.AddKeyValue(1, 3)) test++;
   if (!tree.AddKeyValue(3, 4)) test++;
   if (!tree.AddKeyValue(6, 5)) test++;
   if (!tree.AddKeyValue(7, 7)) test++;
   if (!tree.AddKeyValue(5, 6)) test++;
   if (!tree.AddKeyValue(12, 8)) test++;

   //======= FIND =======
   // search key which is in the tree
   fainded_node_info = tree.FindNodeByKey (12);
   if (fainded_node_info->Node->NodeKey != 12) test++;
   if (fainded_node_info->NodeHasKey != true) test++;

   // search key which is not in the tree and it tends to the left
   not_fainded_node_info = tree.FindNodeByKey (10);
   if (not_fainded_node_info->Node != fainded_node_info->Node) test++;
   if (not_fainded_node_info->NodeHasKey != false) test++; 
   if (not_fainded_node_info->ToLeft != true) test++;

   // search key which is not in the tree and it tends to the right
   not_fainded_node_info = tree.FindNodeByKey (14);
   if (not_fainded_node_info->Node != fainded_node_info->Node) test++;
   if (not_fainded_node_info->NodeHasKey != false) test++; 
   if (not_fainded_node_info->ToLeft != false) test++; 

   //======= ADD =======
   // when the key is already in the tree
   if (tree.AddKeyValue(4, 5)) test++;
   // add to the left
   if (!tree.AddKeyValue(10, 9)) test++;
   fainded_node_info = tree.FindNodeByKey (10);
   if (fainded_node_info->Node->NodeKey != 10) test++;
   if (!fainded_node_info->NodeHasKey == true) test++;
   // add to the right
   if (!tree.AddKeyValue(14, 10)) test++;
   fainded_node_info = tree.FindNodeByKey (14);
   if (fainded_node_info->Node->NodeKey != 14) test++;
   if (fainded_node_info->NodeHasKey != true) test++;

   BSTNode** nodeList;
   Queue* queue;
   nodeList = tree.DeepAllNodes(0);
   //cout << "In_order : " << "\n";
   //list.print();
   nodeList = tree.DeepAllNodes(1);
   //cout << "Post_order : " << "\n";
   //list.print();
   nodeList = tree.DeepAllNodes(2);
   //cout << "Pre_order : " << "\n";   
   //list.print();
   nodeList = tree.WideAllNodes();
   //list.print();

   //======= MAX/MIN =======  
   if (tree.FinMinMax(tree.Root, true)  != tree.FindNodeByKey(14)->Node) test++;
   if (tree.FinMinMax(tree.Root, false) != tree.FindNodeByKey(1)->Node) test++;   
   node = tree.FindNodeByKey(6)->Node;   
   if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7)->Node) test++;
   if (tree.FinMinMax(node, false) != tree.FindNodeByKey(5)->Node) test++;
   node = tree.FindNodeByKey(4)->Node;   
   if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7)->Node) test++;
   if (tree.FinMinMax(node, false) != tree.FindNodeByKey(1)->Node) test++;

   //====== DELETE & COUNT ======
   if (tree.DeleteNodeByKey(4) != true) test++;
   if (tree.FindNodeByKey(4)->NodeHasKey != false) test++;
   if (tree.Count() != 10) test++;

   if (tree.DeleteNodeByKey(5) != true) test++;
   if (tree.FindNodeByKey(5)->NodeHasKey != false) test++;
   if (tree.Count() != 9) test++;

   if (tree.DeleteNodeByKey(6) != true) test++;
   if (tree.FindNodeByKey(6)->NodeHasKey != false) test++;
   if (tree.Count() != 8) test++;

   if (tree.DeleteNodeByKey(3) != true) test++;
   if (tree.FindNodeByKey(3)->NodeHasKey != false) test++;
   if (tree.Count() != 7) test++;

   if (tree.DeleteNodeByKey(2) != true) test++;
   if (tree.FindNodeByKey(2)->NodeHasKey != false) test++;
   if (tree.Count() != 6) test++;

   if (tree.DeleteNodeByKey(7) != true) test++;
   if (tree.DeleteNodeByKey(1) != true) test++;
   if (tree.DeleteNodeByKey(12) != true) test++;
   if (tree.DeleteNodeByKey(14) != true) test++;
   if (tree.DeleteNodeByKey(10) != true) test++;
   if (tree.Count() != 1) test++;
   
   if (tree.DeleteNodeByKey(8) != true) test++; // delete Root when it is only Root in the tree
   if (tree.Count() != 0) test++;

   if (!tree.AddKeyValue(4, 1)) test++;
   if (!tree.AddKeyValue(3, 1)) test++;    
   if (tree.DeleteNodeByKey(4) != true) test++; // delete Root when it have left child
   if (!tree.AddKeyValue(12, 1)) test++; 
   if (tree.DeleteNodeByKey(3) != true) test++; // delete Root when it have right child
   if (tree.Count() != 1) test++;
   
   return test;
}
