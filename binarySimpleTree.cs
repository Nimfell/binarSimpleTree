using System;
using System.Collections.Generic;

namespace AlgorithmsDataStructures2
{
    public class BSTNode<T>
    {
        public int NodeKey; // ключ узла
        public T NodeValue; // значение в узле
        public BSTNode<T> Parent; // родитель или null для корня
        public BSTNode<T> LeftChild; // левый потомок
        public BSTNode<T> RightChild; // правый потомок	

        public BSTNode(int key, T val, BSTNode<T> parent)
        {
            NodeKey = key;
            NodeValue = val;
            Parent = parent;
            LeftChild = null;
            RightChild = null;
        }
    }

    public class BSTFind<T>
    {
        public BSTNode<T> Node;
        public bool NodeHasKey;
        public bool ToLeft;

        public BSTFind() { Node = null; }
    }

    public class BST<T>
    {
        BSTNode<T> Root; // корень дерева, или null

        public BST(BSTNode<T> node)
        {
            Root = node;
        }

        public BSTFind<T> FindNodeByKey(int key)
        {
            BSTFind<T> info = new BSTFind<T>();
            BSTNode<T> node = find_key(Root, key); // find key or null
           
            info.Node = node;
            if (node.NodeKey == key)
                info.NodeHasKey = true;
            else {
                info.NodeHasKey = false;
                if (key < node.NodeKey)
                    info.ToLeft = true;
                else
                    info.ToLeft = false; }
            return info;
        }

        public bool AddKeyValue(int key, T val)
        {
            if (Root == null) {
                BSTNode<T> node = new BSTNode<T>(key, val, null);
                Root = node;
                return true; }
            BSTFind<T> _info = FindNodeByKey(key);
            if (_info.NodeHasKey)
                return false; // if the key is already there     
            else {
                BSTNode<T> node = new BSTNode<T>(key, val, _info.Node);
                if (_info.ToLeft)
                    _info.Node.LeftChild = node;
                else
                    _info.Node.RightChild = node;
                return true; } 
        }

        public BSTNode<T> FinMinMax(BSTNode<T> FromNode, bool FindMax)
        {
            if (FindMax) {
                if (FromNode.RightChild != null)
                    return FinMinMax(FromNode.RightChild, FindMax); } // MAX
            else {
                if (FromNode.LeftChild != null)
                    return FinMinMax(FromNode.LeftChild, FindMax); } // MIN
            return FromNode;
        }

        public bool DeleteNodeByKey(int key)
        {
            BSTFind<T> _info = FindNodeByKey(key);

            if (_info.NodeHasKey) { // the key was found
                if (_info.Node.RightChild != null) { // remove with transfer of key information     
                
                    BSTNode<T> replacement = FinMinMax(_info.Node.RightChild, false);  // find replacement for node                 
                    _info.Node.NodeKey = replacement.NodeKey;              // replace key
                    _info.Node.NodeValue = replacement.NodeValue;          // replace value

                    link_the_tail(replacement, false); }
                else // only delete the key node 
                {                               
                    link_the_tail(_info.Node, true); // left child to left
                }
                return true;
            }
            else
                return false; // if the node was not found
        }

        public int Count()
        {
            if (Root != null)
                return keys_count(Root); // number of nodes in the tree
            return 0;
        }

        private void link_the_tail(BSTNode<T> parent, bool left_child)
        {
            if (left_child)
            {
                if (parent != Root)
                {
                    if (parent.LeftChild != null)                     // if child is not null
                        parent.LeftChild.Parent = parent.Parent;     // link child to new parent
                    if (parent.Parent.LeftChild == parent)           // what parent link to update?
                        parent.Parent.LeftChild = parent.LeftChild;  // update the left parent link (new child or null) 
                    else
                        parent.Parent.RightChild = parent.LeftChild; // update the right parent link (new child or null) 
                }
                else
                {
                    Root = parent.LeftChild;
                    if (Root != null)
                        Root.Parent = null;
                }
            }
            else // if RightChild
            {
                if (parent.RightChild != null)
                    parent.RightChild.Parent = parent.Parent;

                if (parent.Parent.LeftChild == parent)
                    parent.Parent.LeftChild = parent.RightChild;
                else
                    parent.Parent.RightChild = parent.RightChild;
            }
        }

        private BSTNode<T> find_key(BSTNode<T> node, int key) // search 
        {
            if (key < node.NodeKey)
            {
                if (node.LeftChild != null)
                    return find_key(node.LeftChild, key);
            }
            else
            {
                if (key > node.NodeKey)
                    if (node.RightChild != null)
                        return find_key(node.RightChild, key);
            }
            return node;
        }

        private int keys_count(BSTNode<T> node)
        {
            int s = 0;
            if (node == null)
                return 0;
            s += keys_count(node.LeftChild);
            s += keys_count(node.RightChild);
            s++;
            return s;
        }

    }
}  


namespace BinarTree
{
    using AlgorithmsDataStructures2;
    class Program
    {
        static void Main(string[] args)
        {            
            int test = 0;
            BSTNode<int> node;
            BSTFind<int> fainded_node_info;
            BSTFind<int> not_fainded_node_info;

            BST<int> tree = new BST<int>(new BSTNode<int>(8, 0, null));   // create the tree

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
            if (fainded_node_info.Node.NodeKey != 12) test++;
            if (fainded_node_info.NodeHasKey != true) test++;

            // search key which is not in the tree and it tends to the left
            not_fainded_node_info = tree.FindNodeByKey (10);
            if (not_fainded_node_info.Node != fainded_node_info.Node) test++;
            if (not_fainded_node_info.NodeHasKey != false) test++;
            if (not_fainded_node_info.ToLeft != true) test++;

            // search key which is not in the tree and it tends to the right
            not_fainded_node_info = tree.FindNodeByKey (14);
            if (not_fainded_node_info.Node != fainded_node_info.Node) test++;
            if (not_fainded_node_info.NodeHasKey != false) test++;
            if (not_fainded_node_info.ToLeft != false) test++; 

            //======= ADD =======
            // when the key is already in the tree
            if (tree.AddKeyValue(4, 5)) test++;
            // add to the left
            if (!tree.AddKeyValue(10, 9)) test++;
            fainded_node_info = tree.FindNodeByKey (10);
            if (fainded_node_info.Node.NodeKey != 10) test++;
            if (!fainded_node_info.NodeHasKey == true) test++;
            // add to the right
            if (!tree.AddKeyValue(14, 10)) test++;
            fainded_node_info = tree.FindNodeByKey (14);
            if (fainded_node_info.Node.NodeKey != 14) test++;
            if (fainded_node_info.NodeHasKey != true) test++;

            //======= MAX/MIN =======  
            if (tree.FinMinMax(tree.FindNodeByKey(8).Node, true) != tree.FindNodeByKey(14).Node) test++;
            if (tree.FinMinMax(tree.FindNodeByKey(8).Node, false) != tree.FindNodeByKey(1).Node) test++;   
            node = tree.FindNodeByKey(6).Node;   
            if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7).Node) test++;
            if (tree.FinMinMax(node, false) != tree.FindNodeByKey(5).Node) test++;
            node = tree.FindNodeByKey(4).Node;   
            if (tree.FinMinMax(node, true)  != tree.FindNodeByKey(7).Node) test++;
            if (tree.FinMinMax(node, false) != tree.FindNodeByKey(1).Node) test++;

            //====== DELETE & COUNT ======
            if (tree.DeleteNodeByKey(4) != true) test++;
            if (tree.FindNodeByKey(4).NodeHasKey != false) test++;
            if (tree.Count() != 10) test++;

            if (tree.DeleteNodeByKey(5) != true) test++;
            if (tree.FindNodeByKey(5).NodeHasKey != false) test++;
            if (tree.Count() != 9) test++;

            if (tree.DeleteNodeByKey(6) != true) test++;
            if (tree.FindNodeByKey(6).NodeHasKey != false) test++;
            if (tree.Count() != 8) test++;

            if (tree.DeleteNodeByKey(3) != true) test++;
            if (tree.FindNodeByKey(3).NodeHasKey != false) test++;
            if (tree.Count() != 7) test++;

            if (tree.DeleteNodeByKey(2) != true) test++;
            if (tree.FindNodeByKey(2).NodeHasKey != false) test++;
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
            
        }
    }
}
