#include <stdio.h>

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

// intermediate search result
class BSTFind
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

   BST(BSTNode* node)
   {
      Root = node;
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
         BSTNode* replacement = FinMinMax(_info->Node->RightChild, false);  // find replacement for node
         replacement->Parent->LeftChild = NULL;            // delete replacement node from the tail
         _info->Node->NodeKey = replacement->NodeKey;      // replace key
         _info->Node->NodeValue = replacement->NodeValue;  // replace value
         delete replacement;
         return true;
      }
      else
         return false; // if the node was not found
   }

   int Count()
   { 
      return keys_count(Root); // number of nodes in the tree
   }   

private:
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
   if (fainded_node_info->NodeHasKey != false) test++; 
   if (fainded_node_info->ToLeft != true) test++;

   // search key which is not in the tree and it tends to the right
   not_fainded_node_info = tree.FindNodeByKey (14);
   if (not_fainded_node_info->Node != fainded_node_info->Node) test++;
   if (fainded_node_info->NodeHasKey != false) test++; 
   if (fainded_node_info->ToLeft != false) test++; 

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

   //======= MAX/MIN =======  
   if (tree.FinMinMax(tree.Root, true)  != tree.FindNodeByKey(14)->Node) test++;
   if (tree.FinMinMax(tree.Root, false) != tree.FindNodeByKey(1)->Node) test++;   
   node = tree.FindNodeByKey(6)->Node;   
   if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7)->Node) test++;
   if (tree.FinMinMax(node, false) != tree.FindNodeByKey(5)->Node) test++;
   node = tree.FindNodeByKey(4)->Node;   
   if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7)->Node) test++;
   if (tree.FinMinMax(node, false) != tree.FindNodeByKey(1)->Node) test++;

   //====== DELETE ======
   if (tree.DeleteNodeByKey(4) != true) test++;
   if (tree.FindNodeByKey(4)->NodeHasKey != false) test++;

   //====== COUNT ======
   if (tree.Count() != 10) test++;

   return test;
}


