#include <stdio.h>
#include <stdlib.h>

#define MAX 100

// 定义计算偏移量的宏（替代offsetof）
#define OFFSET_OF(type, member) ((size_t)&((type*)0)->member)
// 定义container_of宏，通过成员地址反推结构体首地址
#define container_of(ptr, type, member) ({ \
    const typeof(((type*)0)->member) *__mptr = (ptr); \
    (type*)((char*)__mptr - OFFSET_OF(type, member)); \
})

// 双向链表表结点类型
struct list_head {
  struct list_head *next, *prev;
};

// 双向链表首部结点类型
struct head {
  struct list_head list;
  int len; 
};

// 栈元素类型
struct element {
  struct list_head list;
  char ch;
};

void check(char[], struct head *); // 括号匹配检查
void push(struct element *, struct head *); 
char pop(struct head *);
int isempty(struct head *); // 栈空判断，为真表示空，否则非空
void clear_stack(struct head *); 

int main() {
  char ch[MAX] = {0}; // 带括号的表达式字符串，不超过100个字符
  struct head h;      // 链表头结点
  int cnt = 1;        // 本程序设定共检查5次

  while(cnt<=5){
    printf("请输入不超过100个字符的带括号表达式\n");
    fgets(ch, 100, stdin);

  //1.要获取一个字符串，需要知道哪些要素？
  ////存在哪里、字符串有多长

    // 初始化链表头部结点，产生一个空栈
    h.list.next = h.list.prev = &(h.list);
    h.len = 0; // 栈中元素个数

  //2.一个指针包含哪几个要素？换而言之，从哪几点可以掌握一个指针包含的所有信息？
  ////指针所占内存单元（仅与系统位数有关）；指针指向内容的类型；指针内存储的内容

  //3.如果有结构体struct Student {char name[20]; int age;};那么通过struct Student型指针p->age，
  //从物理存储角度分析，是如何实现结构体成员访问的？
  ////通过获取成员类型，计算相对于结构体首地址的地址偏移量，如age的偏移量是20个内存单元

  //4.指针所占内存仅与系统位数有关，那么，指针类型这一概念的提出有何意义？
  ////如果不规定指向变量的类型，对于一般指针，执行*p时，将无法确定解引用的 “操作范围”，执行p++时，无法确定步长
  ////对于结构体指针等，执行p->age时，将无法获取结构体成员类型，从而无法计算偏移量，无法正确访问成员

  //5.是将整个数组作为参数传给函数，还是仅将首地址传给函数，这两种在代码的写法上有什么不同？
  ////C 语言中不存在 “将整个数组作为参数传递” 的方式
  ////写法 1：形式上 “传整个数组”（实际仍传首地址）————int arr[]
  ////写法 2：显式 “传首地址”（指针形式）————int *arr


    check(ch, &h); // 检查表达式中括号是否匹配
    clear_stack(&h); 
    cnt++;
  }
  return 0;
}
//6.只定义了struct head类型的结构体h，但没定义该类型的指针，怎么给函数传参？
////&h即可等同于struct head *p
void check(char ch[], struct head *s) {
  char *p, c;
  struct element *e;
  int flag = 0;

  p = ch;
  while (*p) {
    switch (*p) {
    case '(':
    case '[':
    case '{':

  //7.定义  struct element *e 后，
  //定义  struct element list，e=&list 与 定义 e = (struct element *)malloc(sizeof(struct element))有什么区别吗？
  ////list的内存空间由编译器在栈上自动分配（通常在函数调用时分配，函数返回时释放）
  ////malloc 函数手动在堆上申请内存空间，这块空间的大小由 sizeof(struct element) 指定，调用free(e)时才被释放


      e = (struct element *)malloc(sizeof(struct element));
      e->ch = *p++;
      if (s->len <= MAX) {
        // task1: your code goes here
      push(e, s); 

        s->len++;
      } else {
        flag = -1;
        goto out;
      }
      break; 
    case ')':
    case ']':
    case '}':
      if (!isempty(s)) { // 栈不空
          // task2: your code goes here
        c = pop(s);
        s->len--;
        flag = 0;
        if (*p == ')' && c != '(' || *p == ']' && c != '[' ||
            *p == '}' && c != '{') { // 弹出的栈顶元素与*p不配对
          printf("当前%c缺对应的左括号\n", *p); // 栈顶括号c与当前右括号不匹配
          flag = 1;
          goto out;
        } else {
          // 当前括号匹配继续检查下一个括号的匹配情况
          p++;
          break;
        }
      } else { // 栈空
        printf("当前%c缺对应的左括号\n", *p);
        flag = 1;
        goto out;
      }
    default:
      p++; // 其它字符不处理，指针向后移
    }
  }
out:
  if (flag == -1)
    printf("栈已满\n");
  if (!flag && isempty(s)) // 检查栈是否为空
    printf("括号匹配\n");
  if (!isempty(s))
    printf("1到多个左括号不匹配\n");
}

char pop(struct head *head) {
  struct element *entry;
  char c; 

  // task3: your code goes here
  entry = container_of(head->list.prev, struct element, list); // 通过链表结点找到对应的element
  head->list.prev = entry->list.prev; // 调整链表前驱指针
  entry->list.prev->next = &(head->list); // 调整链表后继指针
  c = entry->ch;


  free(entry);

  return c;
}

void push(struct element *e, struct head *head) {
  // task4: your code goes here
  e->list.next = &(head->list);
  e->list.prev = head->list.prev;
  head->list.prev->next = &(e->list);
  head->list.prev = &(e->list);

}

int isempty(struct head *h) {
    // task5: your code goes here; 
  return h->list.next == &(h->list) && h->list.prev == &(h->list);
}

void clear_stack(struct head *s) {
  while (!isempty(s))
    pop(s);
  s->len = 0;
}