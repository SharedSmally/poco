/**
 * Wrapper class of DBusMessage
 * https://dbus.freedesktop.org/doc/api/html/structDBusMessage.html
 * https://dbus.freedesktop.org/doc/api/html/dbus-message_8h_source.html
 *
 */
#ifndef CPW_DBUS_MESSAGE_H
#define CPW_DBUS_MESSAGE_H

#ifndef DBUS_MESSAGE_H
#define DBUS_MESSAGE_H
#include <dbus/dbus-message.h>
#endif

#include <Exception.h>

namespace cpw {
namespace dbus {

class Message : public Wrapper<DBusMessage *>
{
    /** Opaque type representing a message iterator.
     * Can be copied by value, and contains no allocated memory
     * so never needs to be freed and can be allocated on the stack. */
/*
    typedef struct DBusMessage DBusMessage;
    typedef struct DBusMessageIter DBusMessageIter;
*/
    /**
     * DBusMessageIter struct; contains no public fields.
     */

    /**
    DBusMessage* dbus_message_new               (int          message_type);
    */
    Message(int message_type) : shared_(false) {
        ptr_ = dbus_message_new (message_type);
    }
    /**
    DBusMessage* dbus_message_new_method_call   (const char  *bus_name,
                                                 const char  *path,
                                                 const char  *iface,
                                                 const char  *method);
     */
    Message(const char  *bus_name, const char  *path,
            const char  *iface, const char  *method)
    : shared_(false) {
        ptr_ =  dbus_message_new_method_call (bus_name, path, iface, method);
    }

     /**
    DBusMessage* dbus_message_new_method_return (DBusMessage *method_call);
     */

    /**
    DBusMessage* dbus_message_new_signal        (const char  *path,
                                                 const char  *iface,
                                                 const char  *name);
     */
    Message(const char  *path, const char  *iface, const char  *name){
        ptr_ = dbus_message_new_signal (path, iface, name);
    }

     /**
    DBusMessage* dbus_message_new_error         (DBusMessage *reply_to,
                                                 const char  *error_name,
                                                 const char  *error_message);
     */
    Message(Message &reply_to, const char  *error_name, const char  *error_message) {
        ptr_ = dbus_message_new_error(reply_to.ptr(), error_name, error_message);
    }

    /**
    DBusMessage* dbus_message_new_error_printf  (DBusMessage *reply_to,
                                                 const char  *error_name,
                                                 const char  *error_format,
                                                 ...);

     */

    /**
    DBusMessage* dbus_message_copy              (const DBusMessage *message);
     */
    Message(Message & message) {
        ptr_ = dbus_message_copy(message.ptr());
    }

    /**
    DBusMessage*  dbus_message_ref              (DBusMessage   *message);
     */
    Message(DBusMessage *message) {
        ptr_ = dbus_message_ref(message);
    }

    /**
    void          dbus_message_unref            (DBusMessage   *message);
     */
    ~Message() {
        dbus_message_unref(ptr());
    }

    /**
    int           dbus_message_get_type         (DBusMessage   *message);
     */
    int getType() {
        return dbus_message_get_type(ptr());
    }

    /**
    dbus_bool_t   dbus_message_set_path         (DBusMessage   *message,
                                                 const char    *object_path);
     */
    bool setPath(const char    *object_path) {
        return dbus_message_set_path(ptr(), object_path);
    }

    /**
    const char*   dbus_message_get_path         (DBusMessage   *message);
     */
    const char*  getPath() {
        return dbus_message_get_path (ptr());
    }

    /**
    dbus_bool_t   dbus_message_has_path         (DBusMessage   *message,
                                                 const char    *object_path);
     */
    bool hasPath( const char *object_path) {
        return dbus_message_has_path (ptr(), object_path);
    }
    /**
    dbus_bool_t   dbus_message_set_interface    (DBusMessage   *message,
                                                 const char    *iface);
     */
    bool setInterface( const char    *iface) {
        return dbus_message_set_interface(ptr(), iface);
    }

    /**
    const char*   dbus_message_get_interface    (DBusMessage   *message);
     */
    const char* getInterface() {
        return dbus_message_get_interface (ptr());
    }

    /**
    dbus_bool_t   dbus_message_has_interface    (DBusMessage   *message,
                                                 const char    *iface);
     */
    bool hasInterface( const char *iface) {
        dbus_message_has_interface (ptr(), iface);
    }

    /**
    dbus_bool_t   dbus_message_set_member       (DBusMessage   *message,
                                                 const char    *member);
     */
    bool setMember(const char    *member) {
        return dbus_message_set_member (ptr(), member);
    }

    /**
    const char*   dbus_message_get_member       (DBusMessage   *message);
     */
    const char* getMember() {
        return dbus_message_get_member (ptr());
    }

    /**
    dbus_bool_t   dbus_message_has_member       (DBusMessage   *message,
                                                 const char    *member);
     */
    bool hasMember( const char *member) {
        return dbus_message_has_member(ptr(), member);
    }
    /**
    dbus_bool_t   dbus_message_set_error_name   (DBusMessage   *message,
                                                 const char    *name);
     */
    bool setErrorName(const char *name) {
        return dbus_message_set_error_name (ptr(), name);
    }

    /**
    const char*   dbus_message_get_error_name   (DBusMessage   *message);
     */
    const char*getErrorName () {
        return dbus_message_get_error_name (ptr());
    }

    /**
    dbus_bool_t   dbus_message_set_destination  (DBusMessage   *message,
                                                 const char    *destination);
     */
    bool setDestination(const char *destination) {
        return dbus_message_set_destination  (ptr(), destination);
    }


    /**
    const char*   dbus_message_get_destination  (DBusMessage   *message);
     */
    const char* getDestination() {
        return dbus_message_get_destination (ptr());
    }
    /**
    dbus_bool_t   dbus_message_set_sender       (DBusMessage   *message,
                                                 const char    *sender);
     */
    bool setSender(const char *sender) {
        return dbus_message_set_sender (ptr(), sender);
    }

    /**
    const char*   dbus_message_get_sender       (DBusMessage   *message);
     */
    const char* getSender() {
        return dbus_message_get_sender (ptr());
    }

    /**
    const char*   dbus_message_get_signature    (DBusMessage   *message);
     */
    const char* getSignature() {
        return dbus_message_get_signature (ptr());
    }

    /**
    void          dbus_message_set_no_reply     (DBusMessage   *message,
                                                 dbus_bool_t    no_reply);
     */
    void  setNoReply(bool no_reply) {
        dbus_message_set_no_reply (ptr(), no_reply);
    }

    /**
    dbus_bool_t   dbus_message_get_no_reply     (DBusMessage   *message);
     */
    bool getNoReply() {
        return dbus_message_get_no_reply (ptr());
    }

    /**
    dbus_bool_t   dbus_message_is_method_call   (DBusMessage   *message,
                                                 const char    *iface,
                                                 const char    *method);
     */
    bool isMethodCall(const char *iface,  const char *method) {
        return dbus_message_is_method_call (ptr(), iface, method);
    }

    /**
    dbus_bool_t   dbus_message_is_signal        (DBusMessage   *message,
                                                 const char    *iface,
                                                 const char    *signal_name);
     */
    bool isSignal( const char *iface, const char *signal_name) {
        return dbus_message_is_signal (ptr(), iface, signal_name);
    }
    /**
    dbus_bool_t   dbus_message_is_error         (DBusMessage   *message,
                                                 const char    *error_name);
     */
    bool isError( const char *error_name) {
        return dbus_message_is_error (ptr(), error_name);
    }


    /**
    dbus_bool_t   dbus_message_has_destination  (DBusMessage   *message,
                                                 const char    *bus_name);
     */
    bool hasDestination(const char *bus_name) {
        return dbus_message_has_destination  (ptr(), bus_name);
    }

    /**
    dbus_bool_t   dbus_message_has_sender       (DBusMessage   *message,
                                                 const char    *unique_bus_name);
     */
    bool hasSender(const char *unique_bus_name) {
        return dbus_message_has_sender (ptr(), unique_bus_name);
    }

    /**
    dbus_bool_t   dbus_message_has_signature    (DBusMessage   *message,
                                                 const char    *signature);
     */
    bool hasSignature( const char *signature) {
        return dbus_message_has_signature (ptr(), signature);
    }

    /**
    dbus_uint32_t dbus_message_get_serial       (DBusMessage   *message);
     */
    dbus_uint32_t getSerial() {
        return dbus_message_get_serial(ptr());
    }

    /**
    void          dbus_message_set_serial       (DBusMessage   *message,
                                                 dbus_uint32_t  serial);
     */
    void setSerial(dbus_uint32_t  serial) {
        dbus_message_set_serial (ptr(), serial);
    }

    /**
    dbus_bool_t   dbus_message_set_reply_serial (DBusMessage   *message,
                                                 dbus_uint32_t  reply_serial);
     */
    bool setReplySerial(dbus_uint32_t  reply_serial) {
        return dbus_message_set_reply_serial ( ptr(), reply_serial);
    }

    /**
    dbus_uint32_t dbus_message_get_reply_serial (DBusMessage   *message);
     */
    dbus_uint32_t getReplySerial() {
        return dbus_message_get_reply_serial (ptr());
    }
    /**
    void          dbus_message_set_auto_start   (DBusMessage   *message,
                                                 dbus_bool_t    auto_start);
     */
    void setAutoStart(bool auto_start)  {
        dbus_message_set_auto_start (ptr(),  auto_start);
    }

    /**
    dbus_bool_t   dbus_message_get_auto_start   (DBusMessage   *message);
     */
    bool getAutoStart() {
        return dbus_message_get_auto_start (ptr());
    }

    /**
    dbus_bool_t   dbus_message_get_path_decomposed (DBusMessage   *message,
                                                    char        ***path);
     */
    bool getPathDecomposed ( char ** & path) {
        return dbus_message_get_path_decomposed (ptr(),  &path);
    }


    /**
    dbus_bool_t dbus_message_append_args          (DBusMessage     *message,
                                                   int              first_arg_type,
                                                   ...);
     */
    /**
    dbus_bool_t dbus_message_append_args_valist   (DBusMessage     *message,
                                                   int              first_arg_type,
                                                   va_list          var_args);
     */
    bool append(int first_arg_type, va_list  var_args) {
        return dbus_message_append_args_valist (ptr(),  first_arg_type, var_args);
    }

    /**
    dbus_bool_t dbus_message_get_args             (DBusMessage     *message,
                                                   DBusError       *error,
                                                   int              first_arg_type,
                                                   ...);
     */
    /**
    dbus_bool_t dbus_message_get_args_valist      (DBusMessage     *message,
                                                   DBusError       *error,
                                                   int              first_arg_type,
                                                   va_list          var_args);

     */
    /**
    dbus_bool_t dbus_message_contains_unix_fds    (DBusMessage *message);
     */
    bool hasFDs() {
        return dbus_message_contains_unix_fds(ptr());
    }

    class MessageIter : public  Wrapper<DBusMessageIter>
    {
    public:
    /**
    dbus_bool_t dbus_message_iter_init             (DBusMessage     *message,
                                                    DBusMessageIter *iter);
     */
        MessageIter(Message & msg)
            : msg_(msg) {
            bool ret = dbus_message_iter_init (msg.ptr(), ptr());
        }
    /**
    dbus_bool_t dbus_message_iter_has_next         (DBusMessageIter *iter);
     */
        bool hasNext() {
            return dbus_message_iter_has_next (ptr());
        }
    /**
    dbus_bool_t dbus_message_iter_next             (DBusMessageIter *iter);
     */

    /**
    char*       dbus_message_iter_get_signature    (DBusMessageIter *iter);
     */
     char* getSignature() {
         return dbus_message_iter_get_signature (ptr());
     }
    /**
    int         dbus_message_iter_get_arg_type     (DBusMessageIter *iter);
     */
     int getArgType() {
         return dbus_message_iter_get_arg_type (ptr());
     }

    /**
    int         dbus_message_iter_get_element_type (DBusMessageIter *iter);
     */
     int getElementType() {
         return dbus_message_iter_get_element_type (ptr());
     }
    /**
    void        dbus_message_iter_recurse          (DBusMessageIter *iter,
                                                    DBusMessageIter *sub);
     */
    /**
    void        dbus_message_iter_get_basic        (DBusMessageIter *iter,
                                                    void            *value);
    */
     void getBasic(void * value) {
         dbus_message_iter_get_basic (ptr(), value);
     }
/**
    void        dbus_message_iter_get_fixed_array  (DBusMessageIter *iter,
                                                    void            *value,
                                                    int             *n_elements);

     */
     void  getFixedArray(void *value, int &n_element) {
         dbus_message_iter_get_fixed_array ( ptr(), value, &n_element);
     }

    /**
    void        dbus_message_iter_init_append        (DBusMessage     *message,
                                                      DBusMessageIter *iter);
     */

    /**
    dbus_bool_t dbus_message_iter_append_basic       (DBusMessageIter *iter,
                                                      int              type,
                                                      const void      *value);
     */
     bool append( int  type, const void  *value) {
         return dbus_message_iter_append_basic (ptr(), type, value);
     }

    /**
    dbus_bool_t dbus_message_iter_append_fixed_array (DBusMessageIter *iter,
                                                      int              element_type,
                                                      const void      *value,
                                                      int              n_elements);
     */
     bool append(int element_type, const void  *value, int n_elements)
     {
         return dbus_message_iter_append_fixed_array (ptr(),
                 element_type, value, n_elements);
     }

    /**
    dbus_bool_t dbus_message_iter_open_container     (DBusMessageIter *iter,
                                                      int              type,
                                                      const char      *contained_signature,
                                                      DBusMessageIter *sub);
     */
    /**
    dbus_bool_t dbus_message_iter_close_container    (DBusMessageIter *iter,
                                                      DBusMessageIter *sub);
     */
    /**
    void        dbus_message_iter_abandon_container  (DBusMessageIter *iter,
                                                      DBusMessageIter *sub);
    */
    protected:
        Message & msg_;
    };


    /**
    void dbus_message_lock    (DBusMessage  *message);
     */
    void lock() {
        dbus_message_lock (ptr());
    }

    /**
    dbus_bool_t  dbus_set_error_from_message  (DBusError    *error,
                                               DBusMessage  *message);
     */
    bool getError(DBusError & error) {
        return dbus_set_error_from_message (&error, ptr());
    }

    /**
    dbus_bool_t dbus_message_allocate_data_slot (dbus_int32_t     *slot_p);
     */
    static bool allocateSlot(dbus_int32_t & slot) {
        dbus_message_allocate_data_slot (&slot);
    }
    /**
    void        dbus_message_free_data_slot     (dbus_int32_t     *slot_p);
     */
    void freeSlot(dbus_int32_t & slot) {
        dbus_message_free_data_slot (&slot);
    }

    /**
    dbus_bool_t dbus_message_set_data           (DBusMessage      *message,
                                                 dbus_int32_t      slot,
                                                 void             *data,
                                                 DBusFreeFunction  free_data_func);
     */
    bool setData( dbus_int32_t slot, void *data, DBusFreeFunction freeFn) {
        return dbus_message_set_data (ptr(), slot, data, freeFn);
    }

     /**
    void*       dbus_message_get_data           (DBusMessage      *message,
                                                 dbus_int32_t      slot);
     */
    void * getData( dbus_int32_t  slot) {
        return dbus_message_get_data (ptr(), slot);
    }
    /**
    int         dbus_message_type_from_string (const char *type_str);
     */
    static int getType(const char *type_str) {
        return dbus_message_type_from_string (type_str);
    }
    /*
    const char* dbus_message_type_to_string   (int type);
     */
     static const char* getTypeStr(int type) {
         return dbus_message_type_to_string (type);
     }

     /**
    dbus_bool_t  dbus_message_marshal   (DBusMessage  *msg,
                                         char        **marshalled_data_p,
                                         int          *len_p);
     */
    bool marshal( char  * & marshalled_data, int & len) {
        return dbus_message_marshal (ptr(), &marshalled_data, & len);
    }


     /**
    DBusMessage* dbus_message_demarshal (const char *str,
                              int         len,  DBusError  *error);
     */
     Message(const char *str, int len) : shared_(false) {
         Error error;
         ptr_ = dbus_message_demarshal (str, len, error.ptr());
         DBusException::check(error);
     }
     /**
    int  dbus_message_demarshal_bytes_needed (const char *str, int len);
    */
     static int unmarshalBytesNeeded(const char *str, int len) {
         return dbus_message_demarshal_bytes_needed (str, len);
     }
private:
   bool shared_;
};

}
}

#endif
