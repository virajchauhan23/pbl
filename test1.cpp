// #include <iostream>
// #include <cstdio>
// #include <cstdlib>
// #include <cstring>
// #include <cmath>
// #include <ctime>
// #include <vector>
// #include <map>
// #include <set>
// #include <unordered_map>
// #include <unordered_set>
// #include <stack>
// #include <queue>
// #include <bitset>
// #include <array>
// #include <list>
// #include <forward_list>
// #include <tuple>
// #include <optional>
// #include <variant>
// #include <any>
// #include <valarray>
// #include <thread>
// #include <mutex>
// #include <future>
// #include <stdexcept>
// #define PI 3.14
// #define SQUARE(x) ((x)*(x))
// #define MAX(X, Y) ((X) > (Y) ? (X) : (Y))

// using namespace std;

// template <typename T, typename U>
// class Pair {
// public:
//     T first;
//     U second;
// };

// typedef int myint;

// class Base {
// public:
//     virtual void foo() {}
// };

// class Derived : public Base {
// public:
//     void foo() override {}
// };

// int global_var = 100;

// int main() {
//     // Primitive types
//     int i = 10;
//     float f = 3.14f;
//     double d = 6.28;
//     char c = 'A';
//     bool b = true;
//     long l = 100000L;
//     short s = 100;
//     unsigned int ui = 42U;
//     const int ci = 5;

//     // STL
//     vector<int> v = {1, 2, 3};
//     map<string, int> m;
//     set<int> se;
//     unordered_map<int, string> um;
//     unordered_set<int> us;
//     multimap<int, int> mm;
//     multiset<int> ms;
//     stack<int> stk;
//     queue<int> q;
//     priority_queue<int> pq;
//     bitset<8> bits("10101010");
//     array<int, 5> arr = {1, 2, 3, 4, 5};
//     list<int> li;
//     forward_list<int> fl;
//     pair<int, string> p(1, "hello");
//     tuple<int, double, char> t(1, 2.0, 'c');
//     optional<int> opt = 5;
//     variant<int, float> var = 3.14f;
//     any a = 7;
//     valarray<int> va = {1, 2, 3};

//     // Memory
//     int* dyn = new int[5];
//     delete[] dyn;
//     int* malloced = (int*)malloc(sizeof(int) * 5);
//     free(malloced);
//     char dest[20];
//     memcpy(dest, "hello", 6);
//     strcpy(dest, "world");
//     strlen(dest);

//     // Math
//     abs(-5);
//     fabs(-5.5);
//     pow(2, 3);
//     sqrt(16);
//     sin(0);
//     cos(0);
//     tan(0);
//     floor(3.7);
//     ceil(3.2);
//     round(3.5);
//     rand();
//     srand(time(NULL));

//     // Time
//     time_t t_now = time(0);
//     clock_t t_clock = clock();
//     double dt = difftime(t_now, t_now);
//     struct tm* lt = localtime(&t_now);
//     struct tm* gt = gmtime(&t_now);
//     char time_str[100];
//     strftime(time_str, 100, "%Y-%m-%d", lt);

//     // IO Streams
//     cout << "Hello" << endl;
//     cerr << "Error" << endl;
//     cin >> i;
//     getline(cin, c);
//     cout.put('A');
//     cout.get();
//     cout.flush();

//     // Algorithm
//     vector<int> alg = {5, 1, 3};
//     sort(alg.begin(), alg.end());
//     find(alg.begin(), alg.end(), 3);
//     count(alg.begin(), alg.end(), 1);
//     copy(alg.begin(), alg.end(), back_inserter(v));
//     reverse(alg.begin(), alg.end());
//     accumulate(alg.begin(), alg.end(), 0);
//     all_of(alg.begin(), alg.end(), [](int x){ return x > 0; });
//     any_of(alg.begin(), alg.end(), [](int x){ return x > 0; });
//     none_of(alg.begin(), alg.end(), [](int x){ return x < 0; });
//     lower_bound(alg.begin(), alg.end(), 3);
//     upper_bound(alg.begin(), alg.end(), 3);

//     // Strings
//     string str = "123";
//     stoi(str);
//     stof(str);
//     stod(str);
//     to_string(100);
//     strcmp("a", "b");
//     strncmp("abc", "abd", 3);
//     strchr("abc", 'b');
//     strrchr("abcabc", 'a');
//     strstr("hello world", "world");
//     strcat(dest, "suffix");
//     strncat(dest, "abc", 3);

//     // RTTI / Casts
//     Base* base = new Derived();
//     typeid(*base);
//     Derived* dcast = dynamic_cast<Derived*>(base);
//     const int* cptr = &ci;
//     int* ncptr = const_cast<int*>(cptr);
//     float fval = static_cast<float>(i);
//     void* vptr = reinterpret_cast<void*>(&i);

//     // Concurrency
//     mutex mtx;
//     lock_guard<mutex> lg(mtx);
//     unique_lock<mutex> ul(mtx);
//     auto fut = async([](){ return 42; });
//     future<int> future_val = promise<int>().get_future();

//     // Exceptions
//     try {
//         throw runtime_error("fail");
//     } catch (exception& e) {
//         cout << e.what();
//     }

//     // Ternary and logical
//     int x = (i > 5) ? (i < 10 ? 1 : 2) : 3;
//     if (i == 10 && f < 5.0 || c == 'A') {
//         goto end_label;
//     }

//     // Loop and flow
//     for (int j = 0; j < 10; j++) {
//         continue;
//     }

//     while (i--) {
//         break;
//     }

//     do {
//         i++;
//     } while (i < 10);

//     switch (i) {
//         case 0: break;
//         case 1: break;
//         default: break;
//     }

//     return 0;

// end_label:
//     return -1;
// }
