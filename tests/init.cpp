#include <catch.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <RBTree.hpp>

SCENARIO("rbt init", "[init]") {
	RBTree<int> rbt;

	REQUIRE(rbt.count() == 0);
}

SCENARIO("rbt insert", "[insert]") {
	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	std::stringstream ss;
	rbt.inorder(ss);

	std::string str, pattern = "3 8 10 ";
	std::getline(ss, str);

	REQUIRE(str == pattern);
}

SCENARIO("rbt in", "[in]") {
	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	std::stringstream ss;
	rbt.inorder(ss);

	std::string str, pattern = "3 8 10 ";
	std::getline(ss, str);

	REQUIRE(str == pattern);
}

SCENARIO("rbt pre", "[pre]") {
	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	std::stringstream ss;
	rbt.preorder(ss);

	std::string str, pattern = "8 3 10 ";
	std::getline(ss, str);

	REQUIRE(str == pattern);
}

SCENARIO("rbt find", "[find]") {

	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	REQUIRE(rbt.findElement(8) == true);
	REQUIRE(rbt.findElement(3) == true);
	REQUIRE(rbt.findElement(10) == true);
	REQUIRE(rbt.findElement(16) == false);
}

SCENARIO("rbt delete", "[delete]") {
	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	rbt.deleteElement(8);

	std::stringstream ss;
	rbt.inorder(ss);

	std::string str, pattern = "3 10 ";
	std::getline(ss, str);

	REQUIRE(str == pattern);
}

SCENARIO("rbt file write", "[write]") {
	RBTree<int> rbt;
	std::ofstream p("test.rbt");
	p << "3 3 8 10 ";
	p.close();

	std::ifstream in("test.rbt");
	std::ofstream out("test.rbt");

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);

	out << rbt;
	out.close();

	std::stringstream ss;
	rbt.inorder(ss);

	std::string str, pattern = "3 8 3 10 ";
	std::getline(in, str);
	in.close();

	REQUIRE(str == pattern);
}

SCENARIO("rbt file read", "[read]") {
	RBTree<int> rbt;
	std::ofstream p("test.rbt");
	p << "3 3 8 10 ";
	p.close();

	std::ifstream in("test.rbt");

	in >> rbt;
	in.close();

	std::stringstream ss;
	rbt.inorder(ss);

	std::string str, pattern = "3 8 10 ";
	std::getline(ss, str);

	REQUIRE(str == pattern);
}

SCENARIO("rbt print", "[print]") {
	RBTree<int> rbt;

	rbt.insertElement(8);
	rbt.insertElement(3);
	rbt.insertElement(10);
	rbt.insertElement(1);
	rbt.insertElement(6);
	rbt.insertElement(4);
	rbt.insertElement(7);
	rbt.insertElement(14);
	rbt.insertElement(13);

	std::stringstream ss;
	ss << rbt;

	std::string e, str, pattern = "\t\t14r\n\t13b\n\t\t10r\n8b\n\t\t\t7r\n\t\t6b\n\t\t\t4r\n\t3r\n\t\t1b\n\n";

	while (!ss.eof())
	{
		std::getline(ss, e);
		str += e + "\n";
	}

	REQUIRE(str == pattern);
}


//		+----+							+----+
//		|-08-|							|-08-|
//		+----+							+----+
//
//
//					    04
//					   ---->
SCENARIO("rbt delete non inserted element", "[delete]") {
	RBTree<int> rbt = { 8 };
	rbt.deleteElement(4);
	REQUIRE(rbt.count() == 1);
}

//		+----+
//		|-08-|							*
//		+----+
//
//
//					    08
//					   ---->
SCENARIO("rbt delete root without children", "[delete]") {
	RBTree<int> rbt = { 8 };
	rbt.deleteElement(8);
	REQUIRE(rbt.count() == 0);
}

//                +----+                                              +----+
//                |-08-|                                              | 04 |
//                +----+                                              +----+
//                  /                                                   /
//                 /                                                   /
//           +----+                                              +----+
//           | 04 |                                              | 03 |
//           +----+                                              +----+
//             /
//            /
//      +----+                              08
//      | 03 |                             ---->
//      +----+
SCENARIO("rbt delete root with one child", "[delete]") {
	RBTree<int> rbt = { 8, 4, 3 };
	rbt.deleteElement(8);
	REQUIRE(rbt == RBTree<int>({ 4, 3 }));
}

//                +----+                                              +----+
//                |-08-|                                              | 09 |
//                +----+                                              +----+
//                  /\                                                  /\
//                 /  \                                                /  \
//           +----+    +----+                                    +----+    +----+
//           | 04 |    | 10 |                                    | 04 |    | 10 |
//           +----+    +----+                                    +----+    +----+
//             /         /\                                        /          \
//            /         /  \                                      /            \
//      +----+    +----+    +----+          08              +----+              +----+
//      | 03 |    | 09 |    | 13 |         ---->            | 03 |              | 13 |
//      +----+    +----+    +----+                          +----+              +----+
//                            /                                                   /
//                           /                                                   /
//                     +----+                                              +----+
//                     | 11 |                                              | 11 |
//                     +----+                                              +----+
//                        \                                                   \
//                         \                                                   \
//                          +----+                                              +----+
//                          | 12 |                                              | 12 |
//                          +----+                                              +----+
SCENARIO("rbt delete root with children", "[delete]") {
	RBTree<int> rbt = { 8, 4, 3, 10, 9, 13, 11, 12 };
	rbt.deleteElement(8);
	std::cout << rbt;
	std::cout << RBTree<int>({ 9, 4, 11, 3, 10, 13, 12 });
	REQUIRE(rbt == RBTree<int>({ 9, 4, 11, 3, 10, 13, 12 }));
}

//                +----+                                              +----+
//                | 08 |                                              | 08 |
//                +----+                                              +----+
//                  /\                                                  /\
//                 /  \                                                /  \
//           +----+    +----+                                    +----+    +----+
//           | 04 |    | 10 |                                    | 04 |    | 10 |
//           +----+    +----+                                    +----+    +----+
//             /         /\                                                  /\
//            /         /  \                                                /  \
//      +----+    +----+    +----+          03                        +----+    +----+
//      |-03-|    | 09 |    | 13 |         ---->                      | 09 |    | 13 |
//      +----+    +----+    +----+                                    +----+    +----+
//                            /                                                   /
//                           /                                                   /
//                     +----+                                              +----+
//                     | 11 |                                              | 11 |
//                     +----+                                              +----+
//                        \                                                   \
//                         \                                                   \
//                          +----+                                              +----+
//                          | 12 |                                              | 12 |
//                          +----+                                              +----+
SCENARIO("rbt delete non root without children", "[delete]") {
	RBTree<int> rbt = { 8, 4, 3, 10, 9, 13, 11, 12 };
	rbt.deleteElement(3);
	REQUIRE(rbt == RBTree<int>({ 9, 4, 11, 10, 13, 12, 8 }));
}

//                +----+                                              +----+
//                | 08 |                                              | 08 |
//                +----+                                              +----+
//                  /\                                                  /\
//                 /  \                                                /  \
//           +----+    +----+                                    +----+    +----+
//           | 04 |    | 10 |                                    | 04 |    | 10 |
//           +----+    +----+                                    +----+    +----+
//             /         /\                                        /         /\
//            /         /  \                                      /         /  \
//      +----+    +----+    +----+          11              +----+    +----+    +----+
//      | 03 |    | 09 |    | 13 |         ---->            | 03 |    | 09 |    | 13 |
//      +----+    +----+    +----+                          +----+    +----+    +----+
//                            /                                                   /
//                           /                                                   /
//                     +----+                                              +----+
//                     |-11-|                                              | 12 |
//                     +----+                                              +----+
//                        \
//                         \
//                          +----+
//                          | 12 |
//                          +----+
SCENARIO("rbt delete non root with one child", "[delete]") {

	RBTree<int> rbt = { 8, 4, 3, 10, 9, 13, 11, 12 };
	rbt.deleteElement(12);
	REQUIRE(rbt == RBTree<int>({ 9, 4, 11, 10, 13, 8, 3 }));
}

//                +----+                                              +----+
//                | 08 |                                              | 08 |
//                +----+                                              +----+
//                  /\                                                  /\
//                 /  \                                                /  \
//           +----+    +----+                                    +----+    +----+
//           | 04 |    |-10-|                                    | 04 |    | 11 |
//           +----+    +----+                                    +----+    +----+
//             /         /\                                        /         /\
//            /         /  \                                      /         /  \
//      +----+    +----+    +----+          10              +----+    +----+    +----+
//      | 03 |    | 09 |    | 13 |         ---->            | 03 |    | 09 |    | 13 |
//      +----+    +----+    +----+                          +----+    +----+    +----+
//                            /                                                   /
//                           /                                                   /
//                     +----+                                              +----+
//                     | 11 |                                              | 12 |
//                     +----+                                              +----+
//                        \
//                         \
//                          +----+
//                          | 12 |
//                          +----+
SCENARIO("rbt delete non root with children", "[delete]") {

	RBTree<int> rbt = { 8, 4, 3, 10, 9, 13, 11, 12 };
	rbt.deleteElement(10);
	std::cout << rbt;
	std::cout << RBTree<int>({ 9, 4, 11, 12, 13, 8, 3 });
	REQUIRE(rbt == RBTree<int>({ 9, 4, 11, 12, 13, 8, 3 }));
}
