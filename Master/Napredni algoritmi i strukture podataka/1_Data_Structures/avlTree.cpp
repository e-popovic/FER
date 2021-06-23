#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>
#include <utility> 
using namespace std;

struct tree_node {
    char c;
    tree_node *left_node=nullptr,*right_node=nullptr;
    tree_node(char c) {
        this->c=c;
    }
    ~tree_node() {
        if(left_node!=nullptr) delete left_node;
        if(right_node!=nullptr) delete right_node;
    }
};

struct tree {
    unordered_map<char,tree_node*> _tree;
    tree_node *root_node=nullptr;
    void add_root(char root) {
        root_node=new tree_node(root);
        _tree[root]=root_node;
    }
    void add_root(tree_node *root) {
        if(_tree.find(root->c)==_tree.end()) _tree[root->c]=root;
        root_node=root;
    }
    void add_child(char parent,char child,bool left) {
        if(_tree.find(parent)!=_tree.end()) {
            tree_node *cn=new tree_node(child);
            _tree[child]=cn;
            if(left) _tree[parent]->left_node=cn;
            else _tree[parent]->right_node=cn;
        }
    }
    void add_child(tree_node *parent,tree_node *child,bool left) {
        if(_tree.find(parent->c)==_tree.end()) _tree[parent->c]=parent;
        if(_tree.find(child->c)==_tree.end()) _tree[child->c]=child;
        if(left) parent->left_node=child;
        else parent->right_node=child;
    }
    tree_node *get_parent(tree_node *n) {
        for(pair<char,tree_node*> n1:_tree)
            if((n1.second->left_node!=nullptr && n1.second->left_node->c==n->c) ||
              (n1.second->right_node!=nullptr && n1.second->right_node->c==n->c)) return n1.second;
        return nullptr;
    }
    ~tree() {
        if(root_node!=nullptr) delete root_node;
    }
    string traverse_pre_order() {
        string s="";
        this->_traverse_pre_order(root_node,&s);
        return s;
    }
    void _traverse_pre_order(tree_node *node,string *s) {
        if(node==nullptr) return;
        (*s)+=node->c; // append wasn't working right on GCC
        this->_traverse_pre_order(node->left_node,s);
        this->_traverse_pre_order(node->right_node,s);
    }
    string traverse_post_order() {
        string s="";
        this->_traverse_post_order(root_node,&s);
        return s;
    }
    void _traverse_post_order(tree_node *node,string *s) {
        if(node==nullptr) return;
        this->_traverse_post_order(node->left_node,s);
        this->_traverse_post_order(node->right_node,s);
        (*s)+=node->c;
    }
    tree_node *_constructTree(string pre_order,string post_order,int *pindex,int l, int h) {
        if (*pindex>=pre_order.size() || l>h) return nullptr;
      
        tree_node *root=new tree_node(pre_order[*pindex]);
        (*pindex)++;
      
        if(l==h) return root;
      
        int i;
        for (i=l;i<=h;i++)
            if(pre_order[*pindex]==post_order[i]) break;
        if(i<=h) {
            if((i+1)>(h-1)) {
                if(pre_order[*pindex-1]>pre_order[*pindex]) add_child(root, _constructTree(pre_order,post_order,pindex,l,i), true);
                else add_child(root, _constructTree(pre_order,post_order,pindex,l,i), false);
            } else {
                add_child(root, _constructTree(pre_order,post_order,pindex,l,i), true);
                add_child(root, _constructTree(pre_order,post_order,pindex,i+1,h-1), false);
            }
        }
        return root;
    }
    void constructTree(string pre_order,string post_order) {
        int pindex = 0;
        post_order=post_order.substr(0,post_order.size()-1);
        add_root(_constructTree(pre_order,post_order,&pindex,0,(int)post_order.size()));
    }
    void printTree() {
        _printTree("",root_node,true);
    }
    void _printTree(string p,tree_node *root,bool isLeft) {
        cout << p;
        cout << (!isLeft ? "├──" : "└──" );
        if(root!=nullptr) {
            cout << root->c << endl;
            
            if(root->right_node || root->left_node) {
                _printTree(p+(!isLeft ? "│   ":"    "),root->right_node,false);
                _printTree(p+(!isLeft ? "│   ":"    "),root->left_node,true);
            }
        } else cout << endl;
    }
};

class AVLTree
{
private:

    /* racuna visinu cvora */
    int calculateHeight(tree_node *node) {
      int height = 0;
      if (node) {
        int left = 0, right = 0;
        if (node->left_node) {
          left=calculateHeight(node->left_node);
        }
        if (node->right_node) {
          right=calculateHeight(node->right_node);
        }
        height += max(left, right) + 1;
      } 
      return height;
    }

    /* smjesta cvor u drvo */
    void findPlaceForNode(char key, tree_node *parent_node) {
      if (parent_node->c > key) {
        if ((parent_node->left_node) == nullptr){
          my_tree.add_child(parent_node->c, key, true);
          return;
        }
        else {
          findPlaceForNode(key, parent_node->left_node);
          int right_height = 0; 
          int left_height = calculateHeight(parent_node->left_node);
          if (parent_node->right_node) {
            right_height += calculateHeight(parent_node->right_node);
          }
          int fr = right_height - left_height;
          if (fr < -1) {
            if (key < parent_node->left_node->c){
              RightRotation(parent_node);
            }
            else {
              LeftRightRotation(parent_node);
            }
          }

          return;
        }
        
      }
      else {
        if ((parent_node->right_node) == nullptr){
          my_tree.add_child(parent_node->c, key, false);
          return;
        }
        else {
          findPlaceForNode(key, parent_node->right_node);
           
          int left_height = 0;
          if (parent_node->left_node) {
            left_height = calculateHeight(parent_node->left_node);
          }
          int right_height = calculateHeight(parent_node->right_node);
          int fr = right_height - left_height;
          if (fr > 1) {
            if (key > parent_node->right_node->c){
              LeftRotation(parent_node);
            }
            else {
              RightLeftRotation(parent_node);
            }
          }
          return;
        }
        
      }
    }

    void RightLeftRotation(tree_node* my_node) {
      tree_node* child_node;
      child_node = my_node->right_node;
      RightRotation(child_node);
      LeftRotation(my_node);
    }

    void LeftRightRotation(tree_node* my_node) {
      tree_node* child_node;
      child_node = my_node->left_node;
      LeftRotation(child_node);
      RightRotation(my_node);
    }

    void RightRotation(tree_node* my_node) {
      tree_node* replacement_node;
      tree_node* parent_node;
      if (*(this->getRootNode()) != my_node->c){      
        parent_node = my_tree.get_parent(my_node);
      }
      
      replacement_node = my_node->left_node;
      
      my_node->left_node = replacement_node->right_node;
      
      replacement_node->right_node = my_node;
      
      if (*(this->getRootNode()) != my_node->c){      
        if (parent_node->left_node){
          if (parent_node->left_node->c == my_node->c) {
            parent_node->left_node = replacement_node;
          }
          else{
            parent_node->right_node = replacement_node;
          }
          
        } 
        else{
          parent_node->right_node = replacement_node;
        }
      }
      else{      
        my_tree.add_root(replacement_node);
      }
      return; 
    }

    void LeftRotation(tree_node* my_node) {
      tree_node* replacement_node;
      tree_node* parent_node;
      if (*(this->getRootNode()) != my_node->c){      
        parent_node = my_tree.get_parent(my_node);
      }
      replacement_node = my_node->right_node;
      my_node->right_node = replacement_node->left_node;
      replacement_node->left_node = my_node;

      if (*(this->getRootNode()) != my_node->c){     
         if (parent_node->left_node){
           if (parent_node->left_node->c == my_node->c){
             parent_node->left_node = replacement_node;
           }
           else{
              parent_node->right_node = replacement_node;
            }
          
        } 
        else{
          parent_node->right_node = replacement_node;
        }
      }
      else{      
        my_tree.add_root(replacement_node);
      }
      return; 
    }
    
public:

    tree my_tree;

    /* stvaranje stabla*/
    AVLTree(){
    }

    /* Stavlja element u stablo */
    void InsertElement(char key) {
      /* korijenski el */
      if (!(my_tree.root_node)){
        my_tree.add_root(key);
        return;
      }
      
      /* smjesti cvor u stablo */
      findPlaceForNode(key, my_tree.root_node);
      return;
    }

    /* Vraca djecu zadanog cvora */
    pair<char*, char*> getChildrenNodesValues(char *key = nullptr){
      pair<char*, char*> children;
      tree_node *parent_node;
      if (*key) {
        if(my_tree._tree.find(*key)==my_tree._tree.end()){
          children.first = nullptr;
          children.second = nullptr;
          return children;
        }
        parent_node = my_tree._tree.at(*key);
      }
      else {
        parent_node = my_tree.root_node;
      }

      if ((parent_node->left_node == nullptr) || !(parent_node->left_node->c)) {
        children.first = nullptr;
      }
      else {
        char *pointer_value_L;
        pointer_value_L = &(parent_node->left_node->c);
        children.first = pointer_value_L;
      }

      if ((parent_node->right_node == nullptr) || !(parent_node->right_node->c) ) {
        children.second = nullptr;
      }
      else {
        char *pointer_value_R;
        pointer_value_R = &(parent_node->right_node->c);
        children.second = pointer_value_R;
      }

      return children;
    }

    /* Vraca pokazivac na vrijednost korijena */
    char *getRootNode(){
      char *pointer_na_root_value;
      if (this->my_tree.root_node) {
        if (this->my_tree.root_node->c) {
          pointer_na_root_value = &(this->my_tree.root_node->c);
        }
        else {
          pointer_na_root_value = nullptr;
        }
      }
      else {
        pointer_na_root_value = nullptr;
      }
      return pointer_na_root_value;
   }

    /* Preorder prolazak po stablu, vraca string */
    string PreOrderTraversal(){
      string output = this->my_tree.traverse_pre_order();
      return output;
    }
    
    /* Postorder prolazak po stablu, vraca string */
    string PostOrderTraversal(){
      string output = this->my_tree.traverse_post_order();
      return output;
    }

};

int main() 
{ 
  AVLTree *drvo = new AVLTree();

  drvo->InsertElement('L');
  drvo->InsertElement('D');
  drvo->InsertElement('B');
  drvo->InsertElement('C');
  drvo->InsertElement('G');
  drvo->InsertElement('E');
  drvo->InsertElement('J');
  drvo->InsertElement('H');
  drvo->InsertElement('K');
  drvo->InsertElement('V');
  drvo->InsertElement('O');
  drvo->InsertElement('N');
  drvo->InsertElement('M');
  drvo->InsertElement('R');
  drvo->InsertElement('P');
  drvo->InsertElement('W');
  drvo->InsertElement('Y');

  drvo->my_tree.printTree();

  string pre = drvo->PreOrderTraversal();
  string post = drvo->PostOrderTraversal();
  cout << pre;
  cout<<"\n";
  cout<<post;
  return 0;
} 