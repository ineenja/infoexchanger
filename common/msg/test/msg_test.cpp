#include <gtest/gtest.h>
#include <iostream>
#include <list>
#include <mylist/mylist.h>

//////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ТЕСТЫ ДЛЯ КОНСТРУКТОРОВ ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

///// тест конструктора по умолчанию
TEST(MyListTests, Constructor1IsEmpty) {

    MyList<int> myList;
    std::list<int> testList;

    EXPECT_EQ(myList.empty(), testList.empty());
}

///// тест конструктора, заполняющего список count количеством копий элемента со значением value
 TEST(MyListTests, Constructor2HoldValues) {

     int size = 2;
     int value = 1;
     MyList<int> myList(size,value);
     std::list<int> testList(size, value);

     EXPECT_EQ(myList.front(), testList.front());
     EXPECT_EQ(myList.back(), testList.back());
 }

<<<<<<< HEAD
 ///////////////////////////////////////////////////////////////////////////////////////////////
 ////////////////////////////// ТЕСТЫ ДЛЯ ОПЕРАТОРА ПРИСВАИВАНИЯ ///////////////////////////////
 ///////////////////////////////////////////////////////////////////////////////////////////////

 /// тест оператора присваивания копированием
 TEST(MyListTests, assingOperatorCopying) {

     MyList<int> myList;
     MyList<int> testList;

     myList.pushBack(1);
     myList.pushBack(2);
     myList.pushBack(3);

     testList = myList;

     auto iter1 = myList.begin();
     auto iter2 = testList.begin();

     EXPECT_EQ(*iter1, *iter2); // 1=1
     iter1++; iter2++;
     EXPECT_EQ(*iter1, *iter2); // 2=2
     iter1++; iter2++;
     EXPECT_EQ(*iter1, *iter2); // 3=3
 }

 /// тест оператора присваивания
 TEST(MyListTests, assignOperatorMoving) {

     MyList<int> myList;
     MyList<int> testList;

     myList.pushBack(1);
     myList.pushBack(2);
     myList.pushBack(3);

     EXPECT_EQ(myList.empty(), testList.empty());
 }

=======
///// тест конструктора копирования
TEST(MyListTests, ConstructorCopying) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    MyList<int> testList(myList);

    auto iter1 = myList.begin();
    auto iter2 = testList.begin();

    EXPECT_EQ(*iter1, *iter2);
    iter1++; iter2++;
    EXPECT_EQ(*iter1, *iter2);
    iter1++; iter2++;
    EXPECT_EQ(*iter1, *iter2);

    EXPECT_EQ(myList.getSize(), testList.getSize());
}

///// тест конструктора перемещения
TEST(MyListTests, ConstructorMoving) {

    MyList<int> originalList;

    originalList.pushBack(1);
    originalList.pushBack(2);
    originalList.pushBack(3);

    MyList<int> myList(std::move(originalList));

    auto iter1 = myList.begin();
    EXPECT_EQ(*iter1, 1);
    iter1++;
    EXPECT_EQ(*iter1, 2);
    iter1++;
    EXPECT_EQ(*iter1, 3);
    EXPECT_EQ(myList.getSize(), 3);
    EXPECT_EQ(originalList.empty(), true);
}

///// тест конструктора через список инициализации
TEST(MyListTests, ConstructorInitList) {

    MyList<int> myList = {1,2,3};

    auto iter1 = myList.begin();
    EXPECT_EQ(*iter1, 1);
    iter1++;
    EXPECT_EQ(*iter1, 2);
    iter1++;
    EXPECT_EQ(*iter1, 3);
    EXPECT_EQ(myList.getSize(), 3);
}
>>>>>>> 1f2899491f7268343dbc8acb727104c7117de149

//////////////////////////////////////////////////////////////////////////////////////////////
/////// ТЕСТЫ ПЕРЕДАЧИ ПОСЛЕДНЕГО ЭЛЕМЕНТА СПИСКА И ДОБАВЛЕНИЯ ЭЛЕМЕНТА В КОНЕЦ СПИСКА ///////
//////////////////////////////////////////////////////////////////////////////////////////////

///// Тест на количество элементов списка после добавления нескольких элементов в его конец
TEST(MyListTests, pushBackCheckSize) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.pushBack(1);
    testList.push_back(1);

    EXPECT_EQ(myList.getSize(), testList.size());
}

///// Тест возможности передачи ссылки на последний элемент списка посредством проверки его значения
TEST(MyListTests, backCheck) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    EXPECT_EQ(myList.back(), testList.back());

    myList.pushBack(2);
    testList.push_back(2);

    EXPECT_EQ(myList.back(), testList.back());
}

///// Тест возможности изменения последнего элемента списка по передаваемой ссылке
TEST(MyListTests, backCheckChangingValueCheck) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    myList.back() += 1;
    testList.back() += 1;

    EXPECT_EQ(myList.back(), testList.back());
}

/////////////////////////////////////////////////////////////////////////////////////////////
////// ТЕСТЫ ВОЗВРАЩЕНИЯ ПЕРВОГО ЭЛЕМЕНТА СПИСКА И ДОБАВЛЕНИЯ ЭЛЕМЕНТА В НАЧАЛО СПИСКА //////
/////////////////////////////////////////////////////////////////////////////////////////////

///// Проверка размера списка после добавления элементов в начало
TEST(MyListTests, pushFrontCheckSize) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushFront(1);
    testList.push_front(1);

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.pushFront(1);
    testList.push_front(1);

    EXPECT_EQ(myList.getSize(), testList.size());
}

///// Тест возможности передачи ссылки на первый элемент списка посредством проверки его значения
TEST(MyListTests, frontCheck) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    EXPECT_EQ(myList.front(), testList.front());

    myList.pushBack(2);
    testList.push_back(2);

    EXPECT_EQ(myList.front(), testList.front());
}

///// Тест возможности изменения значения первого элемента списка по передаваемой ссылке на него
TEST(MyListTests, frontCheckChangingValueCheck) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    myList.back() += 1;
    testList.back() += 1;

    EXPECT_EQ(myList.front(), testList.front());
}

////////////////////////////////////////////////////////////
/// ТЕСТЫ УДАЛЕНИЯ ПЕРВОГО И ПОСЛЕДНЕГО ЭЛЕМЕНТОВ СПИСКА ///
////////////////////////////////////////////////////////////

///// Проверка количества элементов после удаления последних элементов списка,
///// и не будет ли ошибки при удалении последнего элемента у пустого списка
TEST(MyListTests, popBackCheckSize) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    myList.pushBack(1);
    testList.push_back(1);

    myList.popBack();
    testList.pop_back();

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.popBack();
    testList.pop_back();

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.popBack();
}

///// Проверка количества элементов после удаления первых элементов списка
///// и не будет ли ошибки после удаления первого элемента у пустого списка
TEST(MyListTests, popFrontCheckSize) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    myList.pushBack(1);
    testList.push_back(1);

    myList.popFront();
    testList.pop_front();

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.popFront();
    testList.pop_front();

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.popFront();
}

///// Тест функции очищения списка
TEST(MyListTests, clearListCheck) {

    MyList<int> myList;
    std::list<int> testList;

    myList.clearList(); // вызов очистки пустого списка
    testList.clear();

    myList.pushBack(1);
    testList.push_back(1);

    myList.pushBack(1);
    testList.push_back(1);

    myList.clearList(); // обычная очистка списка
    testList.clear();

    EXPECT_EQ(myList.getSize(), testList.size());

    myList.clearList(); // попытка очистить список, который уже был очищен
    testList.clear();
}

/////////////////////////////////////////////////////////////////////////////////////
//////////////////// ТЕСТЫ ФУНКЦИЙ НА РАБОТУ С ЭЛЕМЕНТАМИ СПИСКА ////////////////////
/////////////////////////////////////////////////////////////////////////////////////

///// тест функции замены содержимого пустого списка на некоторое количество одинаковых элементов
TEST(MyListTests, assignCheckEmpty) {

    MyList<int> myList;
    std::list<int> testList;

    myList.assign(2, 2);
    testList.assign(2, 2);

    EXPECT_EQ(myList.front(), testList.front());
    EXPECT_EQ(myList.back(), testList.back());
}

///// тест функции замены содержимого не пустого списка на некоторое количество одинаковых элементов
TEST(MyListTests, assignCheckNotEmpty) {

    MyList<int> myList;
    std::list<int> testList;

    myList.pushBack(1);
    testList.push_back(1);

    myList.pushBack(1);
    testList.push_back(1);

    myList.assign(2, 2);
    testList.assign(2, 2);

    EXPECT_EQ(myList.front(), testList.front());
    EXPECT_EQ(myList.back(), testList.back());
}

///// тест функции замены содержимого не пустого списка на некоторое количество одинаковых элементов
TEST(MyListTests, assignOperatorCopyingCheck) {

    MyList<int> myList;
    MyList<int> testList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    testList = myList;

    auto iter1 = myList.begin();
    auto iter2 = testList.begin();

    EXPECT_EQ(1, *iter2); // 1 элемент
    iter1++; iter2++;
    EXPECT_EQ(*iter1, *iter2); // 2 элемент
    iter1++; iter2++;
    EXPECT_EQ(*iter1, *iter2); // 3 элемент
}

///// тест функции вставки нового узла перед узлом на который указывает итератор
TEST(MyListTests, insertOneElementCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(3);

    auto iter1 = myList.begin();
    iter1++;

    iter1 = myList.insert(iter1, 2);

    EXPECT_EQ(*iter1, 2);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
}

///// тест функции вставки count новых узлов перед узлом на который указывает итератор
TEST(MyListTests, insertManyElementsCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(1);

    auto iter = myList.begin();
    iter++;

    myList.insert(iter, 2, 2);

    iter = myList.begin();
    EXPECT_EQ(1, *iter);
    iter++;
    EXPECT_EQ(2, *iter);
    iter++;
    EXPECT_EQ(2, *iter);
    iter++;
    EXPECT_EQ(1, *iter);

    EXPECT_EQ(4, myList.getSize());
}

///// тест функции вставки новых узлов из init списка перед узлом на который указывает итератор
TEST(MyListTests, insertElementFromListCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(5);

    auto iter = myList.begin();
    iter++;

    iter = myList.insert(iter, {2,3,4});

    EXPECT_EQ(2, *iter);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }

    EXPECT_EQ(5, myList.getSize());
}

///// тест функции удаления узла, вариант когда узел в середине
TEST(MyListTests, eraseSingleElement1) {

    MyList<int> myList = {1, 2, 3, 3, 4};

    auto iter = myList.begin();
    iter.forward(3);

    iter = myList.erase(iter);
    EXPECT_EQ(4, *iter);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(4, myList.getSize());
}

///// тест функции удаления узла, вариант когда узел - первый
TEST(MyListTests, eraseSingleElement2) {

    MyList<int> myList = {1, 2, 3, 4, 5};

    auto iter = myList.begin();

    iter = myList.erase(iter);
    EXPECT_EQ(2, *iter);

    int check = 2;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(4, myList.getSize());
}

///// тест функции удаления узла, вариант когда узел - последний
TEST(MyListTests, eraseSingleElement3) {

    MyList<int> myList = {1, 2, 3, 4, 5};

    auto iter = myList.begin();
    iter.forward(4);

    iter = myList.erase(iter);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(4, myList.getSize());
}

/// тест функции удаления узлов в диапазоне [first, last) !!!!!valgring problems!!!!!!
TEST(MyListTests, eraseElementsInRange) {

    MyList<int> myList = {1, 2, 3, 3, 3, 4, 5};

    auto iterFirst = myList.begin();
    iterFirst.forward(3);
    auto iterLast = myList.begin();
    iterLast.forward(5);

    auto iter = myList.erase(iterFirst, iterLast);
    EXPECT_EQ(4, *iter);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(5, myList.getSize());
}

///// тест функции удаления узлов в диапазоне [first, last), частный случай first == last
TEST(MyListTests, eraseElementsInRangeOfZero) {

    MyList<int> myList = {1, 2, 3, 4, 5};

    auto iterFirst = myList.begin();
    iterFirst.forward(3);
    auto iterLast = myList.begin();
    iterLast.forward(3);

    auto iter = myList.erase(iterFirst, iterLast);
    EXPECT_EQ(4, *iter);

    int check = 1;
    for (auto iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(5, myList.getSize());
}

///// тест функции замены узла, случай когда заменяемый узел посередине
TEST(MyListTests, emplaceCheck) {

    MyList<int> myList = {1, 2, 3, 5, 5};

    auto iter = myList.begin();
    iter.forward(4);

    iter = myList.emplace(iter, 4);
    EXPECT_EQ(4, *iter);

    int check = 1;
    for (iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(5, myList.getSize());
}

///// тест функции замены узла, случай когда заменяемый узел последний
TEST(MyListTests, emplaceLastCheck) {

    MyList<int> myList = {1, 2, 3, 4, 4};

    auto iter = myList.end();

    iter = myList.emplace(iter, 5);
    EXPECT_EQ(5, *iter);

    int check = 1;
    for (iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(5, myList.getSize());
}

///// тест функции замены узла, случай когда итератор показывает что заменяемый узел стоит перед первым
TEST(MyListTests, emplaceBeforeFirstCheck) {

    MyList<int> myList = {1, 2, 3, 4, 5};

    auto iter = myList.begin();

    iter = myList.emplace(iter, 0);
    EXPECT_EQ(0, *iter);

    int check = 0;
    for (iter = myList.begin(); iter != myList.end(); iter++) {
        EXPECT_EQ(check, *iter);
        check++;
    }
    EXPECT_EQ(6, myList.getSize());
}

///// тест оператора присваивания копированием
TEST(MyListTests, opertorAssignCopyCheck) {

    MyList<int> myList1 = {1, 2, 3, 4, 5};
    MyList<int> myList2 = {2,3};

    myList2 = myList1;
    auto iter2 = myList2.begin();

    for (auto iter1 = myList1.begin(); iter1 != myList1.end(); iter1++) {
        EXPECT_EQ(*iter1, *iter2);
        iter2++;
    }
}

///// тест оператора присваивания перемещением
TEST(MyListTests, opertorAssignMoveCheck) {

    MyList<int> myList1 = {1, 2, 3, 4, 5};
    MyList<int> myList2 = {1,5};

    myList2 = std::move(myList1);

    int check = 1;
    for (auto iter2 = myList2.begin(); iter2 != myList2.end(); iter2++) {
        EXPECT_EQ(check, *iter2);
        check++;
    }
    EXPECT_EQ(myList1.empty(), true);
}

///// тест оператора присваивания с помощью списка инициализации
TEST(MyListTests, opertorAssignInitListCheck) {

    MyList<int> myList = {1, 4, 5};

    myList = {1,2,3,4,5};

    int check = 1;
    for (auto iter1 = myList.begin(); iter1 != myList.end(); iter1++) {
        EXPECT_EQ(*iter1, check);
        check++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ТЕСТЫ ИТЕРАТОРА //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

///// тест возвращения итератора на первый узел списка и оператора разыменования
 TEST(MyListTests, beginCheck) {

     MyList<int> myList;

     myList.pushBack(1);
     myList.pushBack(2);
     myList.pushBack(3);

     auto iter = myList.begin();

     EXPECT_EQ(myList.front(), *iter);
 }

 ///// тест возвращения итератора на следующий после последнего узел списка
  TEST(MyListTests, endCheck) {

      MyList<int> myList;

      myList.pushBack(1);
      myList.pushBack(2);
      myList.pushBack(3);

      auto iter = myList.end();

      bool flagNull = true;
      if (iter.get() == nullptr) {
          flagNull = false;
      }
      EXPECT_EQ(false, flagNull);
  }

///// тест возвращения итератора на первый узел списка и оператора разыменования
TEST(MyListTests, rBeginCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    auto iter = myList.rBegin();

    EXPECT_EQ(myList.back(), *iter);
}

///// тест возвращения итератора на предшествующий первому узел списка
TEST(MyListTests, rEndCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    auto iter = myList.rEnd();

    bool flagNull = true;
    if (iter.get() == nullptr) {
        flagNull = false;
    }

    EXPECT_EQ(false, flagNull);
}

  ///// тест префиксного инкремента
   TEST(MyListTests, prefixIncrementCheck) {

       MyList<int> myList;

       myList.pushBack(1);
       myList.pushBack(2);
       myList.pushBack(3);

       auto iter = myList.begin();
       EXPECT_EQ(1, *iter);

       iter++;
       EXPECT_EQ(2, *iter);

       iter++;
       EXPECT_EQ(3, *iter);
   }

   ///// тест постфиксного инкремента
    TEST(MyListTests, postfixIncrementCheck) {

        MyList<int> myList;

        myList.pushBack(1);
        myList.pushBack(2);
        myList.pushBack(3);

        auto iter = myList.begin();

        ++iter;
        EXPECT_EQ(2, *iter);

        ++iter;
        EXPECT_EQ(3, *iter);
    }

    ///// тест префиксного декремента
     TEST(MyListTests, prefixDecrementCheck) {

         MyList<int> myList;

         myList.pushBack(1);
         myList.pushBack(2);
         myList.pushBack(3);

         auto iter = myList.rBegin();
         EXPECT_EQ(3, *iter);

         iter--;
         EXPECT_EQ(2, *iter);

         iter--;
         EXPECT_EQ(1, *iter);
     }

     ///// тест постфиксного инкремента
      TEST(MyListTests, postfixDecrementCheck) {

          MyList<int> myList;

          myList.pushBack(1);
          myList.pushBack(2);
          myList.pushBack(3);

          auto iter = myList.rBegin();
          EXPECT_EQ(3, *iter);

          --iter;
          EXPECT_EQ(2, *iter);

          --iter;
          EXPECT_EQ(1, *iter);
      }

///// тест функции переноса итератора вперед
TEST(MyListTests, iteratorForwardCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    auto iter = myList.begin();
    iter.forward(1); // перенос итератора с 1 на 2 элемент
    EXPECT_EQ(2, *iter);

}

///// тест функции переноса итератора вперед
TEST(MyListTests, iteratorBackwardCheck) {

    MyList<int> myList;

    myList.pushBack(1);
    myList.pushBack(2);
    myList.pushBack(3);

    auto iter = myList.rBegin();
    iter.backward(1); // перенос итератора с 3 на 2 элемент
    EXPECT_EQ(2, *iter);

}
