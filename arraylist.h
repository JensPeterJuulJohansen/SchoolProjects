#ifndef ARRAYLIST_H
#include <iostream>
#define ARRAYLIST_H

template <typename T>

class ArrayList {

public:

// Denne constructor husker member variablerne.
ArrayList()
{
    mReserved = 1;
    mElems = new T[1];
    mSize= 0;
}

// Copy constructor
// Der kopieres data fra det forhenvaerende object til det nyoprettet object ved brug af for loop.
// Det er muligt at tilgaa member variable direkte, pga. der arbejdes i headeren - kan ogsaa goeres i CPP filen.

ArrayList(const ArrayList<T>& c) {
    mReserved = c.mReserved;
    mSize = c.mSize;
    mElems = new T[c.mReserved]; // mReserved da det reserverede elementer i arrayet.
    for (int i = 0; i < mSize; ++i) {
        mElems[i] = c.mElems[i];
    }
}

// Move constructor
// Her flyttes data fra det forhenvaerende object til det nyoprettet object ved brug af midlertidige variabler.
// I modsaetning til copy constructeren maa den originale data ikke vaere den samme som foer flytning.

ArrayList(ArrayList<T>&& c) {
    int tempReserved = mReserved;
    int tempSize = mSize;
    T *tempElems = mElems;

    mElems = c.mElems;
    mReserved = c.mReserved;
    mSize = c.mSize;

    c.mElems = tempElems;
    c.mReserved = tempReserved;
    c.mSize = tempSize;
}

/*

* Constructor with initialization of "initialized" elements

*/

// Objectet bliver created ved brug af et parameter.

ArrayList(int initialized) {
    mReserved = initialized;
    mSize = 0;
    mElems = new T[initialized];
}

// Destructoren destruerer objectet.

virtual ~ArrayList() {
    delete [] mElems;
    delete &mReserved;
    delete &mSize;
}

/*

* Copy assignment operator

*/

// Her overwrites operatoren til at kunne kopierer et ArrayList object til et andet ArrayList object.
// Hvis der ikke er nok reserveret plads koeres funktionen extendStorage().

ArrayList<T>& operator=(const ArrayList<T>& a) {
    while (mReserved < a.mSize) {
        extendStorage();
    }

    mSize = a.mSize;

    for (int i = 0; i < a.mSize; ++i) {
        mElems[i] = a.mElems[i];
    }

    return *this; // Pointer til det nye object.
}

/*

* Move assignment operator

*/

// Her overwrites operatoren til at kunne flytte et ArrayList object til et andet ArrayList object.

ArrayList<T>& operator=(ArrayList<T>&& a) {
    int tempReserved = mReserved;
    int tempSize = mSize;
    T *tempElems = mElems;

    mElems = a.mElems;
    mReserved = a.mReserved;
    mSize = a.mSize;

    a.mElems = tempElems;
    a.mReserved = tempReserved;
    a.mSize = tempSize;

    return *this;
}

/*

* Add element to dynamic array

*/

// Her tilfoejes et nyt element til slutningen af elementerne i arrayet.
// Hvis der ikke er nok reserveret plads koeres funktionen extendStorage().

void add(const T& element) {

    if (mSize == mReserved)
    extendStorage();

    mElems[mSize] = element;

    ++mSize;
}

/*

* Inserts the element at placement "idx" in array and moves the remaining

* items by one place, restoring the old element at "idx".

* check whether it is needed to extend the storage.

* move all elements from _size to idx (reverse) one element to the right in the array

* set _elems[idx] equal to the element to be inserted

*/

// Her bliver et tempArray created, som husker mElems i det foerste for loop. Elementet bliver flyttet ind paa det valgte index.
// Det andet for loop bruges til at flytte de resterende elementer et index frem i mElems og mSize bliver opdateret.
// tempArray bliver slettet for at frigive hukommelse.
// Hvis der ikke er nok reserveret plads koeres funktionen extendStorage().

void add(int idx, const T& element) {
    if (mSize == mReserved) {
        extendStorage();
    }

    T *tempArray = new T[mSize];

    for (int i = idx; i < mSize; ++i) {
        tempArray[i] = mElems[i];
    }

    mElems[idx] = element;

    for (int i = idx; i < mSize; ++i) {
        mElems[i + 1] = tempArray[i];
    }

    ++mSize;

    delete [] tempArray;
}

/*

* Get a const reference to the element at idx

*/

// Her overwrites operatoren til at returnere et element fra mElems paa et specifikt index med reference.

const T& operator[](int idx) const{

    return &mElems[idx];
}

/*

* Get a reference to the element at idx

*/

// Her overwrites operatoren til at returnere et element fra mElems paa et specifikt index.

T& operator[](int idx){
    return mElems[idx];
}

/*

* Removes the element at placement "idx" by moving all the remaining elements

* by one place to the left in the array

*/

// Her fjernes et element fra et speficikt index, og her benyttes et for loop til at flytte elementerne
// et index til hoejre. Ligeledes decrementeres mSize.

void remove(int idx){
    for (int i = idx; i < mSize; ++i) {
        mElems[i] = mElems[i + 1];
    }

    --mSize;
}

/*

* Returns the number of elements stored

*/

// En getter funktion til at returnerer mSize variablen.

int size() const{
    return mSize;
}

/*

* Returns the number of items currently reserved in memory

*/

// En getter funktion til at returnerer mReserved variablen.

int reserved() const{
    return mReserved;
}

/*

* Returns true if number of elements in array is zero

*/

// En funktion til at se, om der er elementer tilbage i arrayet.
// Her returneres TRUE hvis der ingen elementer er. Ellers FALSE hvis der er.

bool isEmpty() const{
    if (mSize == 0) {
        return true;
    } else {
        return false;
    }
}

/*

* Trims the storage array to the exact number of elements

* stored.

*/

// Forkorte arrayet saa mReserved passer med mSize. Her kreares et nyt array, til at lagre dataen fra mElems,
// saaledes mElems kan slettes, og pladsen kan blive fri. Efterfoelgende saettes mElems pointeren lig med NewArray pointeren.

void trimToSize(){

    mReserved = mSize;

    T *NewArray = new T[mSize];

    for (int i = 0; i < mSize; ++i) {
        NewArray[i] = mElems[i];
    }

    delete [] mElems; // destructor gamle mElems

    mElems = NewArray;
}

/*

* Sorts the array using insertion sort (or another algorithm)

* You are not allowed to use standard algorithms from algorithm header.

*/

// De to for loops benyttes til at sammenligne elementerne i mElems i de forskellige indexer med hinanden.
// Hvis et element er mindre end det andet element, benyttes if statementet hvori en temp variablen kreares til at lagre
// det lille tal. Efterfoelgende lagres det store tal i det lille tals gamle index, hvorefter det store tals index saettes lig
// med temp variablen til at lave en insertion sort.
// Vi har lavet en test af sort til subArray3 i main filen.

void sort(){
    for (int i = 0; i < mSize; ++i) {
        for (int j = 0; j < mSize; ++j) {
            if (mElems[i] < mElems[j]) {
                T temp = mElems[i];
                mElems[i] = mElems[j];
                mElems[j] = temp;
            }
        }
    }
}

/*

* Returns a new ArrayList with elements from "fromIdx" index to "toIdx"

*/

// Laver et nyt object ud fra et eksisterende ArrayList med et start og et slut parameter
// for at de elementer, der skal overfoeres til det nye object. + 1 indgaar da arrays foerste index er 0.

ArrayList<T> subArrayList(int fromIdx, int toIdx) const{

    ArrayList<T> subArrayList(toIdx - fromIdx + 1);
    subArrayList.mReserved = toIdx - fromIdx + 1;
    subArrayList.mSize = toIdx - fromIdx + 1;

    for (int i = 0; i < toIdx - fromIdx + 1; ++i) {
        subArrayList.mElems[i] = mElems[fromIdx + i];
    }

    return subArrayList;
}

/*

* Returns a new C++ style array (copy created with new) with all elements

*/

// Her oprettes et nyt array, der bliver fyldt op med elementerne fra mElems.

T* toArray(){
    T *newArray = new T[mSize];
    for (int i = 0; i < mSize; ++i) {
        newArray[i] = mElems[i];
    }

    return newArray;
}

private:

/*

* extendStorage():

* create new array with size 2*_reserved

* copy old data to the new array

* delete old array

* update pointer _elems to point to the new array

* (Since this method is private, the method will only be used internally,

* but the functionality is needed).

*/

// Hjaelpefunktionen extendStorage bruges til at fordoble den reserverede plads, naar mSize overskrider mReserved.

void extendStorage(){
    mReserved *= 2;
    T* newArray = new T[mReserved];
    for (int i = 0; i < mSize; ++i) {
        newArray[i] = mElems[i];
    }

    delete [] mElems;

    mElems = newArray;
}

/*

* Member variables

*/

// mReserved bruges til at reserverer plads til fremtidige elementer.

int mReserved; // The current capacity of "_elems" array

// mSize: hvor mange elementer der er lagret i mElems.

int mSize; // The number of elements stored

// mElems er arrayet, hvor elementerne bliver lagret.

T* mElems; // Array for storing the elements

};

#endif // ARRAYLIST_H
