#ifndef MYLIST
#define MYLIST

#include <cstddef>
#include <initializer_list>
#include <utility>

////////////////////////////////////////////////////////////////////
///////////////////////// ОПИСАНИЕ СПИСКА //////////////////////////
////////////////////////////////////////////////////////////////////
template <class T>
class MyList {

    ////////////////////////////////////////////////////////////////////
    /////////////////////////// УЗЕЛ СПИСКА ////////////////////////////
    ////////////////////////////////////////////////////////////////////
    struct Node {
        template <typename... Args>
        explicit Node(Args&&... args) noexcept
        : value(std::forward<Args>(args)...), prev(nullptr), next(nullptr) {}

        T value; // копия переданного элемента списка
        Node *prev; // ссылка на предыдущий элемент списка
        Node* next; // ссылка на последующий элемент списка
    };

    Node* first; // ссылка на первый элемент списка
    Node* last; // ссылка на последний элемент списка
    size_t size; // количество элементов списка

public:

    ////////////////////////////////////////////////////////////////
    /////////////////// ПОЛЬЗОВАТЕЛЬСКИЙ ИТЕРАТОР //////////////////
    ////////////////////////////////////////////////////////////////

    struct MyIterator {

        explicit MyIterator(Node* ptr) noexcept : mPtr(ptr) {}

        T& operator*() const noexcept { return mPtr->value; }
        Node* operator->() noexcept { return mPtr; }

        [[nodiscard]] Node* get() const noexcept { return mPtr; }


        // префиксный итератор сложения
        MyIterator& operator++() noexcept {
            if (mPtr) {
                mPtr = mPtr->next; // указатель у итератора переходит на следующий узел
            }
            return *this;
        }

        //постфиксный итератор сложения
        MyIterator operator++(int) noexcept {
        MyIterator temp = *this;
        mPtr = mPtr->next;
        return temp;
        }

        // префиксный итератор вычитания
        MyIterator& operator--() noexcept {
        mPtr = mPtr->prev;
        return *this;
        }

        //постфиксный итератор вычитания
        MyIterator operator--(int) noexcept {
        MyIterator temp = *this;
        mPtr = mPtr->prev;
        return temp;
        }


        MyIterator& operator=(Node* ptr) noexcept {
            mPtr = ptr;
            return *this;
        }

        friend bool operator==(const MyIterator& lhs, const MyIterator& rhs) noexcept { return lhs.mPtr == rhs.mPtr; }
        friend bool operator!=(const MyIterator& lhs, const MyIterator& rhs) noexcept { return !(lhs == rhs); }

        // перенос итератора на step шагов вперед
        void forward(size_t step) noexcept {
            for (size_t i = 0; i < step; ++i) {
                if (mPtr != nullptr) {
                    mPtr = mPtr->next;
                }
            }
        }

        // перенос итератора на step шагов назад
        void backward(size_t step) noexcept {
            for (size_t i = 0; i < step; ++i) {
                if (mPtr != nullptr) {
                    mPtr = mPtr->prev;
                }
            }
        }
    private:
        Node* mPtr;

    };



    //////////////////////////////////////////////////////////////
    /////////////////////// КОНСТРУКТОРЫ /////////////////////////
    //////////////////////////////////////////////////////////////

    // конструктор по умолчанию, создающий пустой контейнер без элементов
    // указателям на первый и последний узлы списка присваиваем nullptr чтобы было ясно что узлов нет
    explicit MyList() noexcept : first(nullptr), last(nullptr), size(0) {};

    // конструктор с числом count копий элемента имеющего значение value
    MyList(size_t count, const T& value) noexcept : MyList(){ // делегируем создание пустого списка конструктору по умолчанию
        this->assign(count, value);
    }

    // конструктор копирования
    MyList(const MyList& other) noexcept : MyList() {
        for (const T& value : other) {
            pushBack(value);
        }
    }

    // конструктор перемещения
    MyList(MyList&& other) noexcept : MyList() {
        size = other.size;
        first = other.first;
        last = other.last;

        other.first = nullptr;
        other.last = nullptr;
        other.size = 0;
    }

    // конструктор для создания контейнера с содержимым списка инициализации init
    MyList(std::initializer_list<T> init) noexcept : MyList() {
        for (const T& value : init) {
            pushBack(value);
        }
    }


    ////////////////////////////////////////////////////////
    ///////////// ДЕСТРУКТОР, ОЧИЩЕНИЕ ПАМЯТИ //////////////
    ////////////////////////////////////////////////////////

    ~MyList() noexcept {
        clearList(); // деструктор списка вызывает функцию очищения списка
        delete first; // удаляем указатель на первый элемент списка
        delete last; // удаляем указатель на последний элемент списка
    }

    void clearList() noexcept { // функция очищения списка
        while (first) { // пока указатель на первый элемент списка на что-то указывает, значит список еще не пуст
            popFront(); // пока список еще не пуст, в цикле будет вызываться функция удаления первого элемента списка
        }
        size = 0;
    }


    ////////////////////////////////////////////////////////
    /////////////////// ОПЕРАТОРЫ СПИСКА ///////////////////
    ////////////////////////////////////////////////////////

    // оператор присваивания копированием
    MyList& operator= (const MyList& other) noexcept {
        if (this != &other) {
            clearList();
            for (auto it = other.begin(); it != other.end(); ++it) {
                pushBack(*it);
            }
        }
        return *this;
    }

    // оператор присваивания перемещением
    MyList& operator= (MyList&& other) noexcept{
        if (this != &other) {
            clearList();
            size = other.size;
            first = other.first;
            last = other.last;

            other.first = nullptr;
            other.last = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // замена текущего содержимого содержимым списка инициализации
    MyList& operator= (std::initializer_list<T> other) noexcept {
        clearList();
        for (const T& value : other) {
            pushBack(value);
        }
        return *this;
    }

    ////////////////////////////////////////////////////////////////////////
    //////////// МЕТОДЫ ДОБАВЛЕНИЯ И УДАЛЕНИЯ ЭЛЕМЕНТОВ(УЗЛОВ) /////////////
    ////////////////////////////////////////////////////////////////////////

    // добавление элемента в конец списка для lvalue
    void pushBack(const T& value) noexcept {
        // выделяем память под новый узел - элемент списка
        // конструктору узла передаем сам элемент, адрес последнего элемента указателю на предыдущий узел,
        // и nullptr указателю на следующий узел, так как мы добавляем элемент в конец списка
        auto* newNode = new Node(value);
        newNode->prev = last;
        if (last) { // проверяем, существует ли последний узел - адресует ли на что-то указатель на последний узел
            last->next = newNode; // если он существует, его указателю на следующий элемент присваиваем адрес нового узла
        } else { // если последнего узла не существует, то делаем вывод, что у нас пустой список
            first = newNode; // присваиваем указателю на первый элемент списка адрес нового узла, который будет первым в списке
        }
        last = newNode; // присваиваем указателю на последний элемент адрес нового узла, так как он теперь и есть последний
        size++; // увеличиваем счетчик узлов списка на 1
    }

    // добавление элемента в конец списка для rvalue
    void pushBack(T&& value) noexcept {
        // выделяем память под новый узел - элемент списка
        // конструктору узла передаем сам элемент, адрес последнего элемента указателю на предыдущий узел,
        // и nullptr указателю на следующий узел, так как мы добавляем элемент в конец списка
        auto* newNode = new Node(std::move(value));
        newNode->prev = last;
        if (last) { // проверяем, существует ли последний узел - адресует ли на что-то указатель на последний узел
            last->next = newNode; // если он существует, его указателю на следующий элемент присваиваем адрес нового узла
        } else { // если последнего узла не существует, то делаем вывод, что у нас пустой список
            first = newNode; // присваиваем указателю на первый элемент списка адрес нового узла, который будет первым в списке
        }
        last = newNode; // присваиваем указателю на последний элемент адрес нового узла, так как он теперь и есть последний
        size++; // увеличиваем счетчик узлов списка на 1
    }

    // добавление элемента в начало списка для lvalue
    void pushFront(const T& value) noexcept {
        // выделяем память под новый узел - элемент списка
        // конструктору узла передаем сам элемент, адрес первого элемента указателю на последующий узел,
        // и nullptr указателю на предыдущий узел, так как мы добавляем элемент в начало списка
        auto* newNode = new Node(value);
        newNode->next = first;
        if (first) { // проверяем, существует ли первый узел - адресует ли на что-то указатель на первый узел
            first->prev = newNode; // если он существует, его указателю на предыдущее значение присваиваем адрес нового узла
        } else { // если первого узла не существует, делаем вывод, что список пуст
            // если список пуст, то новый первый элемент будет и последним, присваиваем указателю на последний узел
            // адрес нового узла
            last = newNode;
        }
        first = newNode; // присваиваем указателю на первый узел адрес нового первого узла
        size++; // увеличиваем счетчик узлов списка на 1
    }

    // добавление элемента в начало списка для rvalue
    void pushFront(T&& value) noexcept {
        // выделяем память под новый узел - элемент списка
        // конструктору узла передаем сам элемент, адрес первого элемента указателю на последующий узел,
        // и nullptr указателю на предыдущий узел, так как мы добавляем элемент в начало списка
        auto* newNode = new Node(std::move(value));
        newNode->next = first;
        if (first) { // проверяем, существует ли первый узел - адресует ли на что-то указатель на первый узел
            first->prev = newNode; // если он существует, его указателю на предыдущее значение присваиваем адрес нового узла
        } else { // если первого узла не существует, делаем вывод, что список пуст
            // если список пуст, то новый первый элемент будет и последним, присваиваем указателю на последний узел
            // адрес нового узла
            last = newNode;
        }
        first = newNode; // присваиваем указателю на первый узел адрес нового первого узла
        size++; // увеличиваем счетчик узлов списка на 1
    }

    // удаление последнего элемента списка
    void popBack() noexcept {
        if (last) { // проверяем, существует ли последний узел - то есть не пустой ли у нас список
            // если список не пустой, то удаление его последнего элемента имеет смысл и можно продолжать работу функции
            Node* temp = last; // создаем временный указатель на последний узел списка
            last = last->prev; // присваиваем указателю на последний узел адрес предпоследнего узла
            // после удаления последнего узла списка, предпоследний узел будет считаться новым последним
            // проверим, существует ли этот предпоследний узел
            if (last) { // если этот узел существует, ...
                last->next = nullptr; // ...удаляем адрес старого последнего узла из указателя нового последнего узла
            } else { // если этого узла не существует, делаем вывод, что в списке лежал только один узел
                // поскольку мы его удаляем, список становится пустым после удаления последнего элемента
                first = nullptr; // указателю на первый узел списка присваиваем пустой адрес, так как после удаления список пуст
            }
            delete temp; // удаляем последний узел, который до этого момента лежал во временном указателе temp
            size--; // уменьшаем счетчик количества узлов на 1
        }
    }

    // удаление первого элемента списка
    void popFront() noexcept {
        if (first) { // проверяем, существует ли первый узел, то есть не пустой ли у нас список
            // если список не пустой, то удаление его первого элемента имеет смысл и можно продолжать работу функции
            Node* temp = first; // создаем временный указатель на первый узел списка
            first = first->next; // присваиваем указателю на первый узел списка адрес второго узла списка
            // после удаления первого узла списка, второму узлу предстоит стать новым первым узлом
            // проверим, существует ли второй узел у нашего списка
            if (first) { // если второй узел у списка есть, ...
                first->prev = nullptr; // ... указателю на предыдущий элемент у второго узла присваиваем пустой адрес
            } else { // если второго узла у списка нет, ...
                // ... после удаления первого и единственного узла списка, наш список станет пустым
                last = nullptr; // поэтому мы присваиваем пустой адрес указателю на последний элемент списка
            }
            delete temp; // удаляем из памяти первый элемент списка, который до этого лежал по временному указателю temp
            size--; // уменьшаем счетчик элементов списка на 1
        }
    }

    // добавление элемента в конец списка без копирования или перемещения
    template <typename... Args>
    void emplaceBack(Args&&... args) noexcept {
        auto* newNode = new Node(std::forward<Args>(args)...);
        if (last) {
            last->next = newNode;
            newNode->prev = last;
            last = newNode;
        } else {
            first = newNode;
            last = newNode;
        }
        size++;
    }

    // добавление элемента в начало списка без копирования или перемещения
    template <typename... Args>
    void emplaceFront(Args&&... args) noexcept {
        auto* newNode = new Node(std::forward<Args>(args)...);
        if (first) {
            first->prev = newNode;
            newNode->next = first;
            first = newNode;
        } else {
            first = newNode;
            last = newNode;
        }
        size++;
    }

    // функция заполнения списка count количеством копий значения value
    void assign(size_t count, const T& value) noexcept {
        clearList(); // предварительное очищение списка
        for (size_t i = 0; i < count; i++) { // заполнение пустого списка копиями value в цикле
            pushBack(value);
        }
    }

    // вставка нового узла на указанную позицию с копированием value
    MyIterator insert(MyIterator pos, const T& value) noexcept {
        // передаваемый итератор pos указывает на узел, перед которым будет вставлен новый узел
        auto newNode = new Node(value); // инициализируем в памяти новый узел и объявляем указатель на него
        if (pos.get() != nullptr) {
            // указателю на следующий элемент нового узла присваиваем адрес узла на который указывает итератор pos
            newNode->next = pos.get();
            // указателю на предыдущий элемент нового узла присваиваем адрес узла, который предшествует узлу на который указывает итератор pos
            newNode->prev = (pos->prev);
            // указателю на следующий узел предыдущего узла присваиваем адрес нового узла
            pos->prev->next = newNode;
            // указателю на предыдущий узел следующего узла присваиваем адрес нового узла
            pos->prev = newNode;
            size++;
        } else {
            pushBack(value); // если итератор указывает на конец списка то просто вставляем новый узел в конец
        }
        return --pos; // возвращаем итератор на новый узел
    }

    // вставка count новых узлов на указанную позицию с копированием value
    void insert(MyIterator pos, size_t count, const T& value) noexcept {
        if (pos.get() != nullptr) {
            for (size_t i = 0; i < count; ++i) {
                insert(pos, value);
            }
        }
        else {
            for (size_t i = 0; i < count; ++i) {
                pushBack(value);
            }
        }
    }

    // вставка новых узлов на указанную позицию из содержимого списка инициализации с копированием value
    MyIterator insert(MyIterator pos, std::initializer_list<T> init) noexcept {
        if (pos.get() != nullptr) {
            for (const T& value : init) {
                insert(pos, value);
            }
        }
        else {
            for (const T& value : init) {
                pushBack(value);
            }
        }
        for (size_t i = 0; i < init.size(); ++i) {
            --pos;
        }

        return pos;
    }

    // удаление узла в позиции pos
    MyIterator erase(MyIterator pos) noexcept {
        if (pos->next != nullptr && pos->prev != nullptr) { // вариант когда удаляемый узел в середине
            Node* temp = pos.get();
            ++pos;
            pos->prev->prev->next = pos.get();
            pos->prev = pos->prev->prev;
            delete temp;
            size--;
            return pos; // тут return внутри условия чтобы не было ошибки сегментации
        }
        if (pos.get() == last) { // вариант когда удаляемый узел - последний
            popBack();
            pos = nullptr;
        }
        if (pos.get() == first) { // вариант когда удаляемый узел - первый
            popFront();
            pos = first;
        }
        return pos;
    }

    // удаление элементов в диапазоне [first, last)
    MyIterator erase(MyIterator firstDeleted, MyIterator lastDeleted) noexcept {
        if (firstDeleted != lastDeleted) {
            while (firstDeleted != lastDeleted) {
                auto temp = firstDeleted;
                ++firstDeleted;
                erase(temp);
            }
        }
        return lastDeleted;
    }

    // замена значения в узле, предыдущего тому на который указывает pos
    MyIterator emplace(MyIterator pos, const T& value) noexcept {
        if (pos.get() == first) {
            pushFront(value);
            pos = first;
        } else {
            if (pos.get() != nullptr) {
                --pos;
                *pos = value;
            } else {
                last->value = value;
                pos = last;
            }
        }
        return pos;
    }

    //////////////////////////////////////////////////
    ////////////// ВОЗВРАЩЕНИЯ ЗНАЧЕНИЙ //////////////
    //////////////////////////////////////////////////

    //доступ к первому и последнему элементов списка
    [[nodiscard]] T& front() const noexcept { return first->value; }
    [[nodiscard]] T& back() const noexcept { return last->value; }

    // получения адресов первого и следующего после последнего элементов списка
    [[nodiscard]] MyIterator begin() const noexcept { return MyIterator(first); }
    [[nodiscard]] MyIterator end() const noexcept { return MyIterator(last->next); }

    // получения адресов стоящего перед первым и последнего элементов списка
    [[nodiscard]] MyIterator rBegin() const noexcept { return MyIterator(last); }
    [[nodiscard]] MyIterator rEnd() const noexcept { return MyIterator(first->prev); }

    [[nodiscard]] bool empty() const noexcept {
        return first == last;
    }

    [[nodiscard]] size_t getSize() const noexcept {
        return size;
    }

};

#endif
