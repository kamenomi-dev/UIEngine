// _Test_DoubleLinkedList.cpp : 此文件包含 "main"
// 函数。程序执行将在此处开始并结束。
//

#include <windows.h>

#include <iostream>

class VAL {
 public:
  int data;
  VAL(int val) : data(val) {}

  ~VAL() { std::cout << "VAL: " << data << " deleted." << std::endl; }
};

struct NodeDataType {
  VAL* data;
  int idx;
  NodeDataType* ParentNode;
  NodeDataType* FirstChildNode;
  NodeDataType* LastChildNode;
  NodeDataType* PrevNode;
  NodeDataType* NextNode;

  NodeDataType() {
    idx = 0;
    data = nullptr;
    ParentNode = FirstChildNode = LastChildNode = PrevNode = NextNode = nullptr;
  }
};

class NodeTree {
 private:
  NodeDataType* m_pRoot = nullptr;

 public:
  NodeTree() { m_pRoot = new NodeDataType; }
  ~NodeTree() { DestroyNodeAndChildren(); }

  auto GetRoot() { return m_pRoot; };

 public:
  NodeDataType* CreateNode(_In_ VAL* data) {
    static int idx = 0;

    auto ptr = new NodeDataType;
    ptr->idx = idx++;
    ptr->data = data;

    return ptr;
  }
  void InsertNode(_Inout_ NodeDataType* node,
                  _Inout_opt_ NodeDataType* parent = nullptr) {
    ExtractNode(node);

    if (parent == nullptr) {
      parent = m_pRoot;
    }
    node->ParentNode = parent;

    if (parent->FirstChildNode == nullptr) {
      parent->FirstChildNode = node;
      parent->LastChildNode = node;

      return;
    }

    if (parent->LastChildNode != nullptr) {
      parent->LastChildNode->NextNode = node;
      node->PrevNode = parent->LastChildNode;

      parent->LastChildNode = node;
    }
  }
  void ExtractNode(_Inout_ NodeDataType* node) {
    if (node->ParentNode != nullptr) {
      auto parent = node->ParentNode;

      if (parent->FirstChildNode == node) {
        parent->FirstChildNode = node->NextNode;
      }

      if (parent->LastChildNode == node) {
        parent->LastChildNode = node->PrevNode;
      }

      node->ParentNode = nullptr;
    }

    if (node->PrevNode != nullptr) node->PrevNode->NextNode = node->NextNode;
    if (node->NextNode != nullptr) node->NextNode->PrevNode = node->PrevNode;

    node->PrevNode = node->NextNode = nullptr;
  }

  void DestroyNodeAndChildren(_Inout_opt_ NodeDataType* node = nullptr) {
    NodeDataType* current = node;
    if (current == nullptr) {
      current = m_pRoot;
    }

    while (current != nullptr) {
      if (current->FirstChildNode != nullptr) {
        DestroyNodeAndChildren(current->FirstChildNode);
      }

      if (current->data != nullptr) {
        delete current->data;
      }

      auto next = current->NextNode;
      delete current;

      current = next;
    }
  }
};

void Print(NodeTree& tree, NodeDataType* target) {
  static unsigned int floor = 1;
  auto current = target->FirstChildNode;

  while (current != nullptr) {
    std::cout << "第" << floor << "层：" << current->data->data << std::endl;
    // std::cout << "------ Target Data Begin ------\n"
    //           << "Address    : " << (int)current << "\n"
    //           << "Data       : " << *(int*)current->data << "\n"
    //           << "Parent     : " << (int)current->ParentNode << "\n"
    //           << "FirstChild : " << (int)current->FirstChildNode << "\n"
    //           << "LastChild  : " << (int)current->LastChildNode << "\n"
    //           << "Prev       : " << (int)current->PrevNode << "\n"
    //           << "Next       : " << (int)current->NextNode << "\n"
    //           << "------- Target Data End -------" << std::endl;

    if (current->FirstChildNode != nullptr) {
      floor++;
      Print(tree, current);
      floor--;
    }

    current = current->NextNode;
  }
  std::cout << "----------------------------------------------------"
            << std::endl;
}

int main() {
  NodeTree tree{};

  auto ptr = tree.CreateNode(new VAL(114514));
  auto ptr1 = tree.CreateNode(new VAL(112));
  auto ptr2 = tree.CreateNode(new VAL(234));
  auto ptr3 = tree.CreateNode(new VAL(6566));
  auto ptr4 = tree.CreateNode(new VAL(2));
  auto ptr5 = tree.CreateNode(new VAL(233));

  tree.InsertNode(ptr);
  tree.InsertNode(ptr1);
  tree.InsertNode(ptr2);

  tree.InsertNode(ptr3, ptr);
  tree.InsertNode(ptr4, ptr3);

  tree.InsertNode(ptr5, ptr2);

  Print(tree, tree.GetRoot());

  std::cout << "Hello World!\n" << std::endl;
  std::cout << "Hello World!\n" << std::endl;
  std::cout << "Hello World!\n" << std::endl;
}