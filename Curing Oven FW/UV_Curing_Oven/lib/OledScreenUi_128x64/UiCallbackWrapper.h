#ifndef CALLBACK_WRAPPER_H
#define CALLBACK_WRAPPER_H

// C-style function pointer to be used when needing to store pointer to UiCallbackWrapper::Execute
typedef void (*UiCallback)(void* context);

/// @brief Generic UI callback wrapper to avoid having to create 
/// wrapper functions manually when creating UI
/// @tparam UiClass Generic type to cast UiImplementation context to
template <class UiClass> struct UiCallbackWrapper 
{
    // Generic C++ style member function pointer from UiClass
    typedef void (UiClass::*MemberFunction)();
    
    /// @brief Stored Object context to call the callback from
    UiClass* object;
    /// @brief member callback function of the saved object 
    MemberFunction memberFunction;
    
    /// @brief Wrapper method to execute the object callback function
    /// @param context UiCallbackWrapper context storing the object and member function
    static void Execute(void* context) 
    {
        // Cast the wrapper
        UiCallbackWrapper<UiClass>* wrapper = static_cast<UiCallbackWrapper<UiClass>*>(context);
        
        // If wrapper, Ui class and Member function pointers are all set, run the function
        if (wrapper != nullptr && 
            wrapper->object != nullptr && 
            wrapper->memberFunction != nullptr) 
        {
            (wrapper->object->*(wrapper->memberFunction))();
        }
    }
};

#endif /*CALLBACK_WRAPPER_H*/

