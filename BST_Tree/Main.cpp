#include <iostream>
#include <fstream>
#include <ctime>
#include "Tree.h"
#include <cmath>
using namespace std;
typedef unsigned long long INT_64;

const int MIN_OPTION = 0;
const int MAX_OPTION = 19;
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

void test_ord(int);
void test_rand(int);
void changeValueByKey(Tree<int, int>& tree);
void removeFromTree(Tree<int, int>&);
void findValuePerKey(Tree<int, int>&);
void fillPreparedTree(Tree<int, int>&);
void fillManually(Tree<int, int>&, const int&);
void fillTree(Tree<int, int>&, Tree<int, int>::Iterator&, Tree<int, int>::Inverse_iterator&);
bool actionByChoice(Tree<int, int>&, const int&, Tree<int, int>::Iterator&, Tree<int, int>::Inverse_iterator&);
int enterNumsAndCheck(const int&, const int&);
int printMenu();

void sRand() {
    srand(time(0));
    RRand = (INT_64)rand();
}

INT_64 LineRand() {
    INT_64 y1, y2;
    y1 = (aRand * RRand + cRand) % mRand;
    y2 = (aRand * y1 + cRand) % mRand;
    RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) > 32;
    return RRand;
}

void test_rand(int n) {
    //�������� ������ ��� 64 � ��������� ������ ���� INT_64
    Tree< INT_64, int > tree;
    //������ ��� ������, ������� ������������ � ������ 
    INT_64* m = new INT_64[n];
    //��������� ������� ���������� �����
    sRand();
    //���������� ������ � ������� ���������� �� ���������� �������
    for (int i = 0; i < n; i++)
    {
        m[i] = LineRand();
        tree.insert(m[i], 1);
    }
    //����� ������� ������ �� �����
    cout << "items count before test:" << tree.size() << endl;
    //��������� ��������� ����������� �������, �������� � ������
    double I = 0;
    double D = 0;
    double S = 0;
    //��������� ������ ��������, 10% - ������� ��������
    for (int i = 0; i < n / 2; i++)
        if (i % 10 == 0)        //10% ��������
        {
            tree.remove(rand());
            D += tree.getLastOperationIterations();

            tree.insert(m[rand() % n], 1);
            I += tree.getLastOperationIterations();

            try {
                tree.get(LineRand());
                S += tree.getLastOperationIterations();
            }
            //��������� ���������� ��� ������ �������� ������
            catch (exception) { S += tree.getLastOperationIterations(); }
        }
        else  //90% �������� ��������
        {
            int ind = rand() % n;
            tree.remove(m[ind]);
            D += tree.getLastOperationIterations();

            INT_64 key = LineRand();
            tree.insert(key, 1);
            I += tree.getLastOperationIterations();
            m[ind] = key;

            try {
                tree.get(m[rand() % n]);
                S += tree.getLastOperationIterations();
            }
            //��������� ���������� ��� ������ �������� ������
            catch (exception) {
                S += tree.getLastOperationIterations();
            }
        }   //����� �����

     //����� �����������:
     //����� ������� ������ ����� �����
    cout << "items count after test:" << tree.size() << endl;
    //������������� ������ ����������� �������� BST
    double thereticalAssessment = 1.39 * (log((double)n) / log(2.0));
    cout << "1.39*log2(n)=" << thereticalAssessment << endl;
    //����������������� ������ ����������� �������
    cout << "Count insert: " << I / (n / 2) << endl;
    //����������������� ������ ����������� ��������
    cout << "Count delete: " << D / (n / 2) << endl;
    //����������������� ������ ����������� ������
    cout << "Count search: " << S / (n / 2) << endl << endl;
    //������������ ������ ������� m[]
    cout << endl << "~~~~~~~~~~~~~~~~~~~~" << endl;
    delete[] m;

    ofstream fout;
    fout.open("test_rand.txt", ios_base::app);
    if (fout.is_open()) {
        fout << n << ' ' << thereticalAssessment << ' ' << I / (n / 2) << ' '
            << D / (n / 2) << ' ' << S / (n / 2) << '\n';
    }
    fout.close();
}

void test_ord(int n) {
    //�������� ������ ��� 64 � ��������� ������ ���� INT_64
    Tree< INT_64, int > tree;
    //������ ��� ������, ������� ������������ � ������ 
    INT_64* m = new INT_64[n];

    //���������� ������ � ������� ���������� � ������������� ������� //������� �� ��������� [0, 10000, 20000, ... ,10000*n]
    for (int i = 0; i < n; i++) {
        m[i] = i * 10000;
        tree.insert(m[i], 1);
    }
    //����� ������� ������ �� ����� 
    cout << "items count before test:" << tree.size() << endl;
    //��������� ��������� ����������� �������, �������� � ������
    double I = 0;
    double D = 0;
    double S = 0;
    //��������� ������� ���������� ����� 
    sRand();
    //��������� ������ ��������, 10% - ������� ��������
    for (int i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0)        //10% ��������
        {
            int k = LineRand() % (10000 * n);
            k = k + !(k % 2);   //��������� �������� ����
            tree.remove(k);
            D += tree.getLastOperationIterations();
            tree.insert(m[rand() % n], 1);
            I += tree.getLastOperationIterations();
            k = LineRand() % (10000 * n);
            k = k + !(k % 2);   // ��������� �������� ����
            try {
                tree.get(k);
                S += tree.getLastOperationIterations();
            }
            //��������� ���������� ��� ������ �������� ������
            catch (exception) { S += tree.getLastOperationIterations(); }
        }
        else  //90% �������� ��������
        {
            int ind = rand() % n;
            tree.remove(m[ind]);
            D += tree.getLastOperationIterations();;
            int k = LineRand() % (10000 * n);
            k = k + (k % 2);        // ��������� ������ ����
            tree.insert(k, 1);
            I += tree.getLastOperationIterations();;
            m[ind] = k;
            try {
                tree.get(m[rand() % n]);
                S += tree.getLastOperationIterations();;
            }
            //��������� ���������� ��� ������ �������� ������
            catch (exception) { S += tree.getLastOperationIterations(); }
        }
    }
    //����� �����������:
    // ����� ������� ������ ����� �����
    cout << "items count after test:" << tree.size() << endl;
    //������������� ������ ����������� �������� BST
    cout << "n/2 =" << n / 2 << endl;
    //����������������� ������ ����������� �������
    cout << "Count insert: " << I / (n / 2) << endl;
    //����������������� ������ ����������� ��������
    cout << "Count delete: " << D / (n / 2) << endl;
    //����������������� ������ ����������� ������
    cout << "Count search: " << S / (n / 2) << endl << endl;
    cout << endl << "~~~~~~~~~~~~~~~~~~~~" << endl;
    //������������ ������ ������� m[]
    delete[] m;
    tree.clear();

    ofstream fout;
    fout.open("test_ord.txt", ios_base::app);
    if (fout.is_open()) {
        fout << n << ' ' << n / 2 << ' ' << I / (n / 2) << ' '
            << D / (n / 2) << ' ' << S / (n / 2) << '\n';
    }
    fout.close();
}   //����� �����

void changeValueByKey(Tree<int, int>& tree) {
    int key, value;


    cout << "~ ������� ���� ������� ����: ";
    key = enterNumsAndCheck(INT_MIN, INT_MAX);

    cout << "~ ������� ����� ��������: ";
    value = enterNumsAndCheck(INT_MIN, INT_MAX);
    tree.get(key).getValue() = value;
}

void removeFromTree(Tree<int, int>& tree) {
    int key;

    cout << "~ ������� ����: ";
    key = enterNumsAndCheck(INT_MIN, INT_MAX);

    tree.remove(key);
}

void addElemToTree(Tree<int, int>& tree) {
    int key, value;

    cout << "~ ������� ����:";
    key = enterNumsAndCheck(INT_MIN, INT_MAX);
    cout << "~ ������� �������� �� �����: ";
    value = enterNumsAndCheck(INT_MIN, INT_MAX);
    tree.insert(key, value);
}

void findValuePerKey(Tree<int, int>& tree) {
    int findKey;
    int value;
    cout << "������� ������� ����";
    findKey = enterNumsAndCheck(INT_MIN, INT_MAX);
    value = tree.get(findKey).getValue();

    if (value)
        cout << "�������� �� ����� " << findKey << " = " << value << endl;
}

void fillPreparedTree(Tree<int, int>& tree) {
    tree.insert(15, 152);
    tree.insert(19, 166);
    tree.insert(13, 176);
    tree.insert(25, 189);
    tree.insert(23, 162);
    tree.insert(35, 198);
    tree.insert(77, 195);
    tree.insert(9, 213);
    tree.insert(50, 123);
    tree.insert(65, 214);
}

void fillManually(Tree<int, int>& tree, const int& SIZE) {
    int value, key;

    for (int i = 0; i < SIZE; i++) {
        cout << "~ ������� ����: ";
        key = enterNumsAndCheck(INT_MIN, INT_MAX);

        cout << "~ ������� �����: ";
        value = enterNumsAndCheck(INT_MIN, INT_MAX);

        tree.insert(key, value);
    }
}

void fillTree(Tree<int, int>& tree, Tree<int, int>::Iterator& iter, Tree<int, int>::Inverse_iterator& invIter) {
    tree.clear();

    int size, item;

    cout << "1. ������������� ������" << endl;
    cout << "2. ��������� ������ ��������������" << endl;

    item = enterNumsAndCheck(1, 2);

    if (item == 1)
        fillPreparedTree(tree);
    if (item == 2) {
        cout << "������� �������� ������ ������: ";
        size = enterNumsAndCheck(1, INT_MAX);

        fillManually(tree, size);
    }

    iter = tree.begin();
    invIter = tree.rBegin();
}

bool actionByChoice(Tree<int, int>& tree, const int& choice, Tree<int, int>::Iterator& iter, Tree<int, int>::Inverse_iterator& invIter) {
    switch (choice) {
    case 0: {
        return true;
    }
    case 1: {
        fillTree(tree, iter, invIter);
        break;
    }

    case 2: {
        cout << "������ ������: " << tree.size();
        break;
    }
    case 3: {
        cout << "������ ������? " << std::boolalpha << tree.isEmpty() << endl;
        break;
    }
    case 4: {
        tree.clear();
        break;
    }
    case 5: {
        tree.print();
        return false;
    }
    case 6: {
        findValuePerKey(tree);
        break;
    }
    case 7: {
        addElemToTree(tree);
        break;
    }
    case 8: {
        removeFromTree(tree);
        break;
    }
    case 9: {
        changeValueByKey(tree);
        break;
    }
    case 10: {
        tree.height();
        break;
    }
    case 11: {
        cout << "���-�� ��������: " << tree.getLastOperationIterations();
        break;
    }
    case 12: {
        iter = tree.begin();
        break;
    }
    case 13: {
        iter++;
        break;
    }
    case 14: {
        iter--;
        break;
    }
    case 15: {
        cout << "���� = " << (*iter)->getKey() << "\t�������� = " << (*iter)->getValue() << endl;
        break;
    }
    case 16: {
        invIter = tree.rBegin();
        break;
    }
    case 17: {
        invIter++;
        break;
    }
    case 18: {
        invIter--;
        break;
    }
    case 19: {
        cout << "���� = " << (*invIter)->getKey() << "\t�������� = " << (*invIter)->getValue() << endl;
        break;
    }
    }

    cout << "\n����� ������" << endl;
    tree.print();
    cout << endl << endl;
    return false;
}

int enterNumsAndCheck(const int& MIN, const int& MAX) {
    int choice;
    while (true) {
        if ((cin >> choice).good() && (choice >= MIN) && (choice <= MAX))
            return choice;

        if (cin.fail())
            cin.clear();

        cin.ignore(100, '\n');
        cout << "�������� ��������, ��������� ������������ �����.\n";
    }
}

int printMenu() {
    cout << endl << "1. ��������� ������" << endl;
    cout << "2. �������� ������ ������" << endl;
    cout << "3. ���������, ������ �� ������" << endl;
    cout << "4. �������� ������" << endl;
    cout << "5. ����� �� �����" << endl;
    cout << "6. �������� �������� �� �����" << endl;
    cout << "7. ������� ������ ��������" << endl;
    cout << "8. �������� �������� �� �����" << endl;
    cout << "9. �������� �������� �� �����" << endl;
    cout << "10. ���������� ������ ������" << endl;
    cout << "11. ����� �������� (����� �������/��������/�������� �� ����������)" << endl;
    cout << "12. ���������� �������� �� ������" << endl;
    cout << "13. �������� �������� �����" << endl;
    cout << "14. �������� �������� �����" << endl;
    cout << "15. �������� �������� �� ������� ���������" << endl;
    cout << "16. ���������� �������� �������� �� ������" << endl;
    cout << "17. �������� �������� �������� �����" << endl;
    cout << "18. �������� �������� �������� �����" << endl;
    cout << "19. �������� �������� �� ������� ��������� ���������" << endl;
    cout << "0. �����" << endl;

    cout << "~ ��� �����: ";
    return enterNumsAndCheck(MIN_OPTION, MAX_OPTION);
}

int main() {
    setlocale(LC_ALL, "rus");
    Tree<int, int> tree;
    Tree<int, int>::Iterator iter(&tree);
    Tree<int, int>::Inverse_iterator invIter(&tree);

    int optionFromMenu;
    bool forExit = false;
    while (!forExit) {
        optionFromMenu = printMenu();
        forExit = actionByChoice(tree, optionFromMenu, iter, invIter);
    }
    tree.clear();


    // ����� �����������
 /*   for (int i = 100, k; i <= 50000; i += 2000)
        test_rand(i);

    for (int i = 1; i <= 10; i++) {
        test_ord(i * 300);
    }*/

    return 0;
}
