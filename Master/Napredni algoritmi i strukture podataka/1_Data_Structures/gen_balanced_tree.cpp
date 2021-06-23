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
void insertElement(char key, tree_node *parent_node, tree* my_tree);
void balanceOutTree(vector<char> *data, int left, int right, tree* my_tree);

void insertElement(char key, tree_node *parent_node, tree* my_tree) {
    if (parent_node->c > key) {
        if ((parent_node->left_node) == nullptr){
          my_tree->add_child(parent_node->c, key, true);
          return;
        }
        else {
          insertElement(key, parent_node->left_node, my_tree);
        }
        
      }
      else {
        if ((parent_node->right_node) == nullptr){
          my_tree->add_child(parent_node->c, key, false);
          return;
        }
        else {
          insertElement(key, parent_node->right_node, my_tree);
        }      
    }
}

void balanceOutTree(vector<char> *data, int left, int right, tree* my_tree) {
    if (left > right)
        return;
    int middle = (left + right) / 2;
    /* korijenski el */
    if ((my_tree->root_node)==nullptr){
        my_tree->add_root(data->at(middle));
    }
    else {
        insertElement(data->at(middle), my_tree->root_node, my_tree);
    }
    balanceOutTree(data, left, middle-1, my_tree);
    balanceOutTree(data, middle+1, right, my_tree);
    return;

}

void preorderString(tree_node* my_node, string *output) {
    if (my_node == nullptr){
        return;
    }
    (*output) += my_node->c; 
    preorderString(my_node->left_node, output);
    preorderString(my_node->right_node, output);
}

void postorderString(tree_node* my_node, string *output) {
    if (my_node == nullptr){
        return;
    } 
    postorderString(my_node->left_node, output);
    postorderString(my_node->right_node, output);
    (*output) += my_node->c;
}

pair<string,string> gen_balanced_tree(vector<char> *data) {
    tree my_tree;
    sort(data->begin(), data->end());
    /*skombaj drvo*/
    if (data){
        int size = data->size();
        balanceOutTree(data, 0, size-1, &my_tree);  
    }
    my_tree.printTree();

    /*ispisi*/
    pair<string,string> ispisi;
    string preorder = "";
    preorderString(my_tree.root_node, &preorder);
    ispisi.first = preorder;

    string postorder = "";
    postorderString(my_tree.root_node, &postorder);
    ispisi.second = postorder;

    return ispisi;
}

int main() {

    vector<char>* v = new vector<char>();
    v->push_back('D');
    v->push_back('E');
    v->push_back('G');
    v->push_back('H');
    v->push_back('I');
    v->push_back('K');
    v->push_back('L');
    v->push_back('M');
    v->push_back('R');
    v->push_back('V');

    pair<string,string> ispisi = gen_balanced_tree(v);
    cout<< ispisi.first + "\n" + ispisi.second;
    
    return 0;
}