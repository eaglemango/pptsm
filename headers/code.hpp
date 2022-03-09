/*!
    @file
    @brief Struct for storing source code
*/
template <class T>
struct Code {
    Code() {
        code = nullptr;
	    size = 0l;
    }

    ~Code() {
        if (code) {
            free(code);

            code = nullptr;
        }
    }

    T* code;
    long int size;    
};
