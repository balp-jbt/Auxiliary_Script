# POSIX

## Basic.cpp

* Funtion: pthread_create( ) pthread_join( )    
* Method: How to pass parameter(s) to sub_func

## Producer_Consumer_Pattern

* Method: How to use multi-threads in class

  * Sub_func in class must be *static*

  * For non-static sub_func, we can pass obj_ptr to an static entrance sub_func

    ```c++
    template <class T>
    void* Consumer<T>::consumer_thread_entrance(void* args) {
        Consumer<T>* consumer = static_cast<Consumer<T>*>(args);
        consumer->consumer_thread();
        pthread_exit(nullptr);
    }
    ```

* Syntax:

  * static member of class must be initialized out of definition, especially may in .cpp(actually in .hpp here)
  * <font color=red>**When using template in C++, definition and implementation must be detached!!!!**</font>
  * Remember to write `#ifndef ...` at the beginning of every .h file 

* Multi-Thread / Pattern

  * Use pthread_mutex_lock and unlock to protect shared sources when visiting them

  *  When calling function uses template as sub_func, remember to add template after it in code

    ```c++
    pthread_create(&worker_tid, NULL, LogFileManager::dump_entry<sint, sgf2n>, (void*) &param); 
    ```

    