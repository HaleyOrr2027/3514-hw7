//vector.hpp

#pragma once

#include <algorithm>  //for std::max
#include <stdexcept>

namespace dsac::array {

template <typename T> // class works for any type. Ex. to create wwe say Vector<int> v
class Vector {

private:
    int cap{0}; //how much memory is allocated. Ex cap = 8 means the vector has space for 8 elements
    int sz{0};  // how many elements are actually stored
    T* data{nullptr}; // pointer to the first element of the array. Ex data = 0x1234 means the first element is stored at memory address 0x1234

public:
    //Constructor. Will initialize the vector with private members.
    Vector() {};
    // Before the constructor runs, they already have values.

    
    //Return the capacity of the vector. const to not modify.
    int capacity() const{
        return cap;
    };

    //elements stored
    int size() const {
        return sz;
    };
    
    // True is empty
    bool empty() const{
        return sz == 0;
    };
    
    //element at index when vector is const
    // Name is odd because C++ lets you define functions for operators like [], +, ==, etc
    // Ex: v[2] translates to v.operator[](2) under the hood
    // When & appears in a type, it means something different. Pointers mean address-of.
    // Here it means reference. A reference is like an alias for a variable. It allows you to access the variable directly without using a pointer.
    const T& operator[](int i) const {
        return data[i];
    };

    
    //element at index when vector is non-const
    T& operator[](int i) {
        return data[i];
    };
    
    // at function for const
    // throw: if index is out of range, throw an exception. This is different from operator[] which does not check for out of range and can lead to undefined behavior.
    const T& at(int i) const {
        if (i < 0 || i >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[i];

    };
    
    // at function for non const
    T& at(int i) {
        if (i < 0 || i >= sz) {
            throw std::out_of_range("Index out of range");
        }
        return data[i];
    };
    
    // first element
    const T& front() const {
        return data[0];
    };

    // first element
    T& front() {
        return data[0];
    };
    
    // last element
    const T& back() const {
        return data[sz - 1];
    };

    // last element
    T& back() {
        return data[sz - 1];
    };


    //capacity >= minimum
    // After calling reserve(minimum), the vector’s capacity must be at least minimum.
    // 
    //     if minimum <= cap
    //     do nothing

    // else
    //     create new array of size minimum
    //     copy old elements
    //     delete old array
    //     update pointer
    //     update capacity
     void reserve(int minimum) {

        if (minimum <= cap) {
            return;
        }

        T* new_data = new T[minimum]; // new  array in heap memory of size minimum

        for (int i = 0; i < sz; i++) {
            new_data[i] = data[i];
        }

        delete[] data; // delete old array. needed bc of "new" / heap

        data = new_data;
        cap = minimum;
    }

    // called by other functions to reduce cap by half 
    // when sz <= cap/4 
    void shrink() {
        if (cap <= 1) {
            return; // don't shrink if capacity is already 1 or less
        }

        if (sz > cap / 4)  {
            return; // no need to shrink if size is greater than cap/4
        }

        int new_cap = cap / 2;
        if (new_cap < 1) new_cap = 1;

        T* new_data = new T[new_cap];
        for (int i = 0; i < sz; i++) new_data[i] = data[i];

        delete[] data;
        data = new_data;
        cap = new_cap;
}

    // insert at end
    void push_back(const T& elem) {
        //check if vector is full first.  
        if (sz == cap) {
            reserve(std::max(1, 2 * cap)); // double the capacity. if cap is 0, set to 1
        }
        data[sz] = elem;
        sz++;
    }

    // remove from end
    // also call shrink
    void pop_back() {
        if (sz == 0) {
            return;
        }
        sz--;
        shrink();
    }

    // insert at index
    void insert(int i, const T& elem) {
        if (i < 0 || i > sz) {
            return;
        }
        if (sz == cap) {
            reserve(std::max(1, 2 * cap));
        }
        for (int j = sz; j > i; j--) {
            data[j] = data[j - 1];
        }
        data[i] = elem;
        sz++;
    }

    // removes at index
    // also call shrink
    void erase(int i) {
        if (i < 0 || i >= sz) {
            return;
        }
        for (int j = i; j < sz - 1; j++) {
            data[j] = data[j + 1];
        }
        sz--;
        shrink();
    }

    
    // explicitly reduce the cap to sz and keep at least 1 slot
    void shrink_to_fit() {
        int new_cap = std::max(1, sz);
        T* new_data = new T[new_cap];

        for (int i = 0; i < sz; i++) {
            new_data[i] = data[i];
        }

        delete[] data;

        data = new_data;
        cap = new_cap;
    }

}; //end class Vector
}//end namespace dsa