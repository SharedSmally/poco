/**
 * Wrapper class of DBusConnection
 * https://dbus.freedesktop.org/doc/api/html/group__DBusBus.html
 * https://dbus.freedesktop.org/doc/api/html/group__DBusConnection.html
 * https://dbus.freedesktop.org/doc/api/html/dbus-shared_8h_source.html
 *
 */
#ifndef CPW_DBUS_CONNECTION_H
#define CPW_DBUS_CONNECTION_H

#ifndef DBUS_DBUS_H
#define DBUS_DBUS_H
#include <dbus/dbus.h>
#endif

#include <Exception.h>

namespace cpw {
namespace dbus {

class Connection : public Wrapper < DBusConnection * >
{
public:
    /**
     * DBusConnection *    dbus_bus_get (DBusBusType type, DBusError *error)
     * DBusConnection *    dbus_bus_get_private (DBusBusType type, DBusError *error)
     *
     * DBUS_BUS_SESSION     The login session bus.
     * DBUS_BUS_SYSTEM      The systemwide bus.
     * DBUS_BUS_STARTER     The bus that started us, if any.
     */
    Connection()
        : shared_(true)
    {
        Error error;
        ptr_ = dbus_bus_get ( DBUS_BUS_STARTER, error.ptr());
        DBusException::check (error);
    }
    /*
     * DBusConnection * dbus_bus_get (DBusBusType type, DBusError *error)
     * DBusConnection * dbus_bus_get_private (DBusBusType type, DBusError *error)
     */
    Connection(bool system_bus, bool private_bus=false)
        : shared_(!private_bus)
    {
        Error error;
        ptr_ = private_bus
                ? dbus_bus_get_private( system_bus ? DBUS_BUS_SYSTEM :  DBUS_BUS_SESSION, error.ptr())
                : dbus_bus_get ( system_bus ? DBUS_BUS_SYSTEM :  DBUS_BUS_SESSION, error.ptr());
        DBusException::check (error);
    }
    /**
     * DBusConnection * dbus_connection_open (const char *address, DBusError *error)
     * DBusConnection * dbus_connection_open_private (const char *address, DBusError *error)
     */
    Connection(const char *address, bool private_bus=false)
        : shared_(!private_bus)
    {
        Error error;
        ptr_ = private_bus ? dbus_connection_open (address, error.ptr())
                : dbus_connection_open_private (address, error.ptr());
        DBusException::check (error);
    }
    /*
     * DBusConnection * dbus_connection_ref (DBusConnection *connection)
     */
    Connection(DBusConnection *connection)
        : shared_(true)
    {
        if (connection == nullptr)
        {
            throw DBusException("Connection(DBusConnection *connection)", "connection is null");
        }

        ptr_ = connection;
        ptr_ = dbus_connection_ref(connection);
    }
    /**
     * void dbus_connection_unref (DBusConnection *connection)
     * void dbus_connection_close (DBusConnection *connection)
     */
    ~Connection()
    {
        if (shared_)
            dbus_connection_unref(ptr());
        else
            dbus_connection_close(ptr());
    }

    /**
enum   DBusWatchFlags {
   DBUS_WATCH_READABLE = 1 << 0,
   DBUS_WATCH_WRITABLE = 1 << 1,
   DBUS_WATCH_ERROR = 1 << 2,
   DBUS_WATCH_HANGUP = 1 << 3
}
    Indicates the status of a DBusWatch.

enum    DBusDispatchStatus {
    DBUS_DISPATCH_DATA_REMAINS,
    DBUS_DISPATCH_COMPLETE,
    DBUS_DISPATCH_NEED_MEMORY
}
    Indicates the status of incoming data on a DBusConnection.
     */

    /**
     * dbus_bool_t     dbus_connection_get_is_connected (DBusConnection *connection)
     *  Gets whether the connection is currently open.
     */
    bool isConnected() {
        dbus_connection_get_is_connected (ptr());
    }

     /**
      * dbus_bool_t     dbus_connection_get_is_authenticated (DBusConnection *connection)
        Gets whether the connection was authenticated.
     */
    bool isAutnenticated() {
        return dbus_connection_get_is_authenticated (ptr());
    }
     /**
      *
    dbus_bool_t     dbus_connection_get_is_anonymous (DBusConnection *connection)
        Gets whether the connection is not authenticated as a specific user.
     */
    bool   isAnonymous () {
        return dbus_connection_get_is_anonymous (ptr());
    }

     /**
      *     char *  dbus_connection_get_server_id (DBusConnection *connection)
        Gets the ID of the server address we are authenticated to, if this connection is on the client side.
          */
    char * getServerID() {
        return  dbus_connection_get_server_id (ptr());
    }

     /**
      *     dbus_bool_t     dbus_connection_can_send_type (DBusConnection *connection, int type)
        Tests whether a certain type can be send via the connection.
          */
    bool canSendType(int type) {
        return dbus_connection_can_send_type (ptr(), type);
    }

     /**
      *     void    dbus_connection_set_exit_on_disconnect (DBusConnection *connection, dbus_bool_t exit_on_disconnect)
        Set whether _exit() should be called when the connection receives a disconnect signal.
         */
    void setExitOnDisconnect(bool flag) {
        dbus_connection_set_exit_on_disconnect (ptr(), flag);
    }

     /**
      *  DBusPreallocatedSend *  dbus_connection_preallocate_send (DBusConnection *connection)
      *      Preallocates resources needed to send a message, allowing the message to be sent without the possibility of memory allocation failure.
      */
    DBusPreallocatedSend * preallocateSend() {
        return dbus_connection_preallocate_send (ptr());
    }

     /**
      * void  dbus_connection_free_preallocated_send (DBusConnection *connection, DBusPreallocatedSend *preallocated)
      *     Frees preallocated message-sending resources from dbus_connection_preallocate_send().
      */
    void freePreallocateSend(DBusPreallocatedSend *preallocated) {
        dbus_connection_free_preallocated_send (ptr(), preallocated);
    }

     /**
      * void  dbus_connection_send_preallocated (DBusConnection *connection, DBusPreallocatedSend *preallocated, DBusMessage *message, dbus_uint32_t *client_serial)
      *     Sends a message using preallocated resources.
      */
    void send(DBusPreallocatedSend *preallocated, DBusMessage *message, dbus_uint32_t & client_serial) {
        dbus_connection_send_preallocated (ptr(), preallocated, message, &client_serial);
    }


     /**
      * dbus_bool_t dbus_connection_send (DBusConnection *connection, DBusMessage *message, dbus_uint32_t *serial)
      *      Adds a message to the outgoing message queue.
      */
    bool send(DBusMessage *message, dbus_uint32_t & serial) {
        return dbus_connection_send (ptr(), message, &serial);
    }

     /**
      *  dbus_bool_t dbus_connection_send_with_reply (DBusConnection *connection, DBusMessage *message, DBusPendingCall **pending_return, int timeout_milliseconds)
      *    Queues a message to send, as with dbus_connection_send(), but also returns a DBusPendingCall used to receive a reply to the message.
      */
    bool sendReply(DBusMessage *message, int timeout_milliseconds, DBusPendingCall * & pending_return) {
        return dbus_connection_send_with_reply (ptr(), message, &pending_return, timeout_milliseconds);
    }

     /**
      * DBusMessage * dbus_connection_send_with_reply_and_block (DBusConnection *connection, DBusMessage *message, int timeout_milliseconds, DBusError *error)
      *   Sends a message and blocks a certain time period while waiting for a reply.
      */

    DBusMessage * sendReply(DBusMessage *message, int timeout_milliseconds) {
        Error error;
        auto ret = dbus_connection_send_with_reply_and_block (ptr(), message,timeout_milliseconds, error.ptr());
        DBusException::check(error);
        return ret;
    }

     /**
      * void    dbus_connection_flush (DBusConnection *connection)
      *     Blocks until the outgoing message queue is empty.
      */
    void flush() {
        dbus_connection_flush (ptr());
    }

     /**
      *  dbus_bool_t dbus_connection_read_write_dispatch (DBusConnection *connection, int timeout_milliseconds)
      *      This function is intended for use with applications that don't want to write a main loop and deal with DBusWatch and DBusTimeout.
      */
    bool readWriteDispatch(int timeout_milliseconds) {
        return dbus_connection_read_write_dispatch (ptr(), timeout_milliseconds);
    }

     /**
      *  dbus_bool_t     dbus_connection_read_write (DBusConnection *connection, int timeout_milliseconds)
      *      This function is intended for use with applications that don't want to write a main loop and deal with DBusWatch and DBusTimeout.
      */
    bool readWrite(int timeout_milliseconds) {
        return dbus_connection_read_write (ptr(), timeout_milliseconds);
    }

     /**
      * DBusMessage *   dbus_connection_borrow_message (DBusConnection *connection)
      *      Returns the first-received message from the incoming message queue, leaving it in the queue.
      */
     DBusMessage * borrow() {
         return dbus_connection_borrow_message (ptr());
     }

     /**
      * void    dbus_connection_return_message (DBusConnection *connection, DBusMessage *message)
      *       Used to return a message after peeking at it using dbus_connection_borrow_message().
      */
     void peekBack( DBusMessage *message) {
         dbus_connection_return_message (ptr(), message);
     }

     /**
      * void    dbus_connection_steal_borrowed_message (DBusConnection *connection, DBusMessage *message)
        Used to keep a message after peeking at it using dbus_connection_borrow_message().
      */
     void peek( DBusMessage *message) {
         dbus_connection_steal_borrowed_message (ptr(), message);
     }


     /**
      * DBusMessage *   dbus_connection_pop_message (DBusConnection *connection)
      *    Returns the first-received message from the incoming message queue, removing it from the queue.
      */
    DBusMessage * pop() {
        return dbus_connection_pop_message (ptr());
    }

     /**
      *  DBusDispatchStatus  dbus_connection_get_dispatch_status (DBusConnection *connection)
          Gets the current state of the incoming message queue.
      */
    DBusDispatchStatus getDispatchStatus() {
        return dbus_connection_get_dispatch_status ( ptr() );
    }

     /**
      *  DBusDispatchStatus  dbus_connection_dispatch (DBusConnection *connection)
        Processes any incoming data.
          */
    DBusDispatchStatus dispatch () {
        return  dbus_connection_dispatch ( ptr() );
    }
     /**
      *  dbus_bool_t dbus_connection_set_watch_functions (DBusConnection *connection,
      *     DBusAddWatchFunction add_function, DBusRemoveWatchFunction remove_function,
      *     DBusWatchToggledFunction toggled_function, void *data, DBusFreeFunction free_data_function)
      *          Sets the watch functions for the connection.
      */
    bool setWatchFns( DBusAddWatchFunction addFn, DBusRemoveWatchFunction removeFn,
                DBusWatchToggledFunction toggledFn, void *data, DBusFreeFunction freeFn) {
        return dbus_connection_set_watch_functions (ptr(),addFn,removeFn, toggledFn, data, freeFn);
    }

     /**
      * dbus_bool_t  dbus_connection_set_timeout_functions (DBusConnection *connection,
      *        DBusAddTimeoutFunction add_function, DBusRemoveTimeoutFunction remove_function,
      *        DBusTimeoutToggledFunction toggled_function, void *data, DBusFreeFunction free_data_function)
      * Sets the timeout functions for the connection.
      */
    bool setTimeoutFns( DBusAddTimeoutFunction addFn, DBusRemoveTimeoutFunction removeFn,
                    DBusTimeoutToggledFunction toggledFn, void *data, DBusFreeFunction freeFn) {
         return  dbus_connection_set_timeout_functions (ptr(), addFn, removeFn, toggledFn, data, freeFn);
     }
     /**
      *  void    dbus_connection_set_wakeup_main_function (DBusConnection *connection,
      *          DBusWakeupMainFunction wakeup_main_function, void *data, DBusFreeFunction free_data_function)
      *  Sets the mainloop wakeup function for the connection.
      */
    void setWakeupFn (DBusWakeupMainFunction wakeupFn, void *data, DBusFreeFunction freeFn) {
        dbus_connection_set_wakeup_main_function (ptr(), wakeupFn, data, freeFn);
    }
     /**
      *
    void    dbus_connection_set_dispatch_status_function (DBusConnection *connection,
          DBusDispatchStatusFunction function, void *data, DBusFreeFunction free_data_function)
        Set a function to be invoked when the dispatch status changes.
          */
    void  setDispatchStatusFn (DBusDispatchStatusFunction fn, void *data, DBusFreeFunction freeFn) {
        dbus_connection_set_dispatch_status_function (ptr(), fn, data, freeFn);
    }



     /**
      *  dbus_bool_t     dbus_connection_get_unix_fd (DBusConnection *connection, int *fd)
        Get the UNIX file descriptor of the connection, if any.
          */
    bool getFD(int & fd) {
        return  dbus_connection_get_unix_fd (ptr(), &fd);
    }

     /**
      *  dbus_bool_t     dbus_connection_get_socket (DBusConnection *connection, int *fd)
        Gets the underlying Windows or UNIX socket file descriptor of the connection, if any.
          */
    bool getSocket(int & fd) {
        return dbus_connection_get_socket ( ptr(), &fd);
    }
     /**
      * dbus_bool_t     dbus_connection_get_unix_user (DBusConnection *connection, unsigned long *uid)
        Gets the UNIX user ID of the connection if known.
          */
    bool getUID( unsigned long & uid) {
        dbus_connection_get_unix_user ( ptr(), &uid);
    }

     /**
      * dbus_bool_t     dbus_connection_get_unix_process_id (DBusConnection *connection, unsigned long *pid)
        Gets the process ID of the connection if any.
          */
    bool getPID(unsigned long & pid) {
        return dbus_connection_get_unix_process_id (ptr(), &pid);
    }
     /**
      *  dbus_bool_t  dbus_connection_get_adt_audit_session_data (DBusConnection *connection, void **data, dbus_int32_t *data_size)
      *    Gets the ADT audit data of the connection if any.
      */
    bool getAdtAuditData(void * & data, dbus_int32_t & data_size) {
        return dbus_connection_get_adt_audit_session_data (ptr(), &data, &data_size);
    }

     /**
      *
    void    dbus_connection_set_unix_user_function (DBusConnection *connection, DBusAllowUnixUserFunction function, void *data, DBusFreeFunction free_data_function)
        Sets a predicate function used to determine whether a given user ID is allowed to connect.
          */
    void setUserFunction(DBusAllowUnixUserFunction fn, void *data, DBusFreeFunction freeFn) {
        dbus_connection_set_unix_user_function (ptr(), fn, data, freeFn);
    }

     /**
      * dbus_bool_t     _dbus_connection_get_linux_security_label (DBusConnection *connection, char **label_p)
      **/
    /*
    bool getSecurityLabel(char * & label) {
        return _dbus_connection_get_linux_security_label (ptr(), &label);
    }*/

     /**
      *
    dbus_bool_t     dbus_connection_get_windows_user (DBusConnection *connection, char **windows_sid_p)
        Gets the Windows user SID of the connection if known.
          */
    bool getWindowUser(char * & windows_sid) {
        return dbus_connection_get_windows_user (ptr(), &windows_sid);
    }

     /**
      *
    void    dbus_connection_set_windows_user_function (DBusConnection *connection,
              DBusAllowWindowsUserFunction function, void *data, DBusFreeFunction free_data_function)
        Sets a predicate function used to determine whether a given user ID is allowed to connect.
          */
    void setWindowUserFn(DBusAllowWindowsUserFunction fn, void *data, DBusFreeFunction freeFn) {
         dbus_connection_set_windows_user_function (ptr(), fn, data, freeFn);
    }
     /**
      *  void dbus_connection_set_allow_anonymous (DBusConnection *connection, dbus_bool_t value)
      *    This function must be called on the server side of a connection when the connection is first seen in the DBusNewConnectionFunction.
      */
      void setAllowAnonymous(bool flag) {
          dbus_connection_set_allow_anonymous (ptr(), flag);
      }

     /**
      * void    dbus_connection_set_route_peer_messages (DBusConnection *connection, dbus_bool_t value)
      *       Normally DBusConnection automatically handles all messages to the org.freedesktop.DBus.Peer interface.
      */
      void setRoutePeerMessage(bool flag) {
          dbus_connection_set_route_peer_messages (ptr(), flag);
      }

     /**
      *  dbus_bool_t     dbus_connection_add_filter (DBusConnection *connection, DBusHandleMessageFunction function,
      *      void *user_data, DBusFreeFunction free_data_function)
      *  Adds a message filter.
      */
      bool addFilter(DBusHandleMessageFunction fn, void *user_data, DBusFreeFunction freeFn) {
          return dbus_connection_add_filter (ptr(), fn, user_data, freeFn);
      }
     /**
      * void dbus_connection_remove_filter (DBusConnection *connection, DBusHandleMessageFunction function, void *user_data)
      *      Removes a previously-added message filter.
      */
      void removeFilter(DBusHandleMessageFunction fn, void * user_data) {
          dbus_connection_remove_filter (ptr(), fn, user_data);
      }

     /**
      *  dbus_bool_t     dbus_connection_try_register_object_path (DBusConnection *connection, const char *path,
      *                const DBusObjectPathVTable *vtable, void *user_data, DBusError *error)
      *   Registers a handler for a given path in the object hierarchy.
      */
    bool try_registerObjectPath( const char *path, const DBusObjectPathVTable & vtable, void *user_data = nullptr) {
        Error error;
        auto ret = dbus_connection_try_register_object_path (ptr(), path, &vtable, user_data, error.ptr());
        DBusException::check(error);
        return ret;
    }

     /**
      *  dbus_bool_t     dbus_connection_register_object_path (DBusConnection *connection,
      *           const char *path, const DBusObjectPathVTable *vtable, void *user_data)
      *     Registers a handler for a given path in the object hierarchy.
      */
    bool registerObjectPath( const char *path, const DBusObjectPathVTable & vtable, void *user_data = nullptr) {
        return dbus_connection_register_object_path (ptr(), path, &vtable, user_data);
    }

     /**
      * dbus_bool_t dbus_connection_try_register_fallback (DBusConnection *connection, const char *path,
      *             const DBusObjectPathVTable *vtable, void *user_data, DBusError *error)
      *  Registers a fallback handler for a given subsection of the object hierarchy.
      */
     bool try_registerFallback(const char *path, const DBusObjectPathVTable & vtable, void *user_data = nullptr) {
         Error error;
         auto ret = dbus_connection_try_register_fallback (ptr(), path, &vtable, user_data, error.ptr());
         DBusException::check(error);
         return ret;
     }

     /**
      *  dbus_bool_t dbus_connection_register_fallback (DBusConnection *connection, const char *path,
      *                const DBusObjectPathVTable *vtable, void *user_data)
      *    Registers a fallback handler for a given subsection of the object hierarchy.
      */
      bool registerFallback(const char *path, const DBusObjectPathVTable & vtable, void *user_data = nullptr) {
              return dbus_connection_register_fallback (ptr(), path, &vtable, user_data);
      }

     /**
      * dbus_bool_t dbus_connection_unregister_object_path (DBusConnection *connection, const char *path)
      *       Unregisters the handler registered with exactly the given path.
      */
     bool unregisterObjectPath(const char *path) {
         return dbus_connection_unregister_object_path (ptr(), path);
     }
     /**
      *  dbus_bool_t  dbus_connection_get_object_path_data (DBusConnection *connection, const char *path, void **data_p)
      *        Gets the user data passed to dbus_connection_register_object_path() or dbus_connection_register_fallback().
      */
     bool getObjectPathData(const char *path, void * & data_p) {
         return dbus_connection_get_object_path_data (ptr(), path, &data_p);
     }
     /**
      *  dbus_bool_t     dbus_connection_list_registered (DBusConnection *connection, const char *parent_path, char ***child_entries)
      *          Lists the registered fallback handlers and object path handlers at the given parent_path.
      */
      bool listRegisterd(const char *parent_path, char ** & child_entries) {
          return dbus_connection_list_registered (ptr(), parent_path, &child_entries);
      }

     /**
      *  dbus_bool_t     dbus_connection_allocate_data_slot (dbus_int32_t *slot_p)
      *     Allocates an integer ID to be used for storing application-specific data on any DBusConnection.
      */

     /**
      * void dbus_connection_free_data_slot (dbus_int32_t *slot_p)
      *      Deallocates a global ID for connection data slots.
      */

     /**
      *  dbus_bool_t  dbus_connection_set_data (DBusConnection *connection, dbus_int32_t slot,
      *             void *data, DBusFreeFunction free_data_func)
      *  Stores a pointer on a DBusConnection, along with an optional function to be used for freeing the data when the data is set again, or when the connection is finalized.
      */
    bool setData(dbus_int32_t slot, void *data, DBusFreeFunction freeFn) {
        return  dbus_connection_set_data (ptr(), slot, data, freeFn);
    }
     /**
      *  void *  dbus_connection_get_data (DBusConnection *connection, dbus_int32_t slot)
      *          Retrieves data previously set with dbus_connection_set_data().
      */
    void * getData(dbus_int32_t slot) {
        return dbus_connection_get_data (ptr(), slot);
    }

     /**
      * void    dbus_connection_set_change_sigpipe (dbus_bool_t will_modify_sigpipe)
      *    This function sets a global flag for whether dbus_connection_new() will set SIGPIPE behavior to SIG_IGN.
      */
    static void setSigPipe(bool modify) {
        dbus_connection_set_change_sigpipe (modify);
    }

     /**
      *  void dbus_connection_set_max_message_size (DBusConnection *connection, long size)
      *          Specifies the maximum size message this connection is allowed to receive.
      */
    void setMaxMessageSize(long size) {
        dbus_connection_set_max_message_size (ptr(), size);
    }

     /**
      * long    dbus_connection_get_max_message_size (DBusConnection *connection)
      *         Gets the value set by dbus_connection_set_max_message_size().
      */
    long getMaxMessageSize() {
        return dbus_connection_get_max_message_size (ptr());
    }

     /**
      * void dbus_connection_set_max_message_unix_fds (DBusConnection *connection, long n)
      *        Specifies the maximum number of unix fds a message on this connection is allowed to receive.
      */
    void setMaxMessageFDs(long n) {
        dbus_connection_set_max_message_unix_fds (ptr(), n);
    }

     /**
      * long dbus_connection_get_max_message_unix_fds (DBusConnection *connection)
      *          Gets the value set by dbus_connection_set_max_message_unix_fds().
      */
    long getMaxMessageFDs() {
        return dbus_connection_get_max_message_unix_fds (ptr());
    }

     /**
      * void dbus_connection_set_max_received_size (DBusConnection *connection, long size)
      *       Sets the maximum total number of bytes that can be used for all messages received on this connection.
      */
    void setMaxRecvSize(long size) {
        dbus_connection_set_max_received_size (ptr(), size);
    }

    /**
     * long    dbus_connection_get_max_received_size (DBusConnection *connection)
     *         Gets the value set by dbus_connection_set_max_received_size().
     */
    long getMaxRecvSize () {
        return dbus_connection_get_max_received_size (ptr());
    }

     /**
      * void dbus_connection_set_max_received_unix_fds (DBusConnection *connection, long n)
      *      Sets the maximum total number of unix fds that can be used for all messages received on this connection.
      */
    void setMaxRecvFDs(long n) {
        dbus_connection_set_max_received_unix_fds (ptr(), n);
    }

     /**
      * long dbus_connection_get_max_received_unix_fds (DBusConnection *connection)
      *      Gets the value set by dbus_connection_set_max_received_unix_fds().
      */
    long getMaxRecvFDs() {
        return dbus_connection_get_max_received_unix_fds (ptr() );
    }

     /**
      * long dbus_connection_get_outgoing_size (DBusConnection *connection)
      *    Gets the approximate size in bytes of all messages in the outgoing message queue.
      */
    long  getOutgoingSize() {
        return dbus_connection_get_outgoing_size ( ptr() );
    }

     /**
      * long dbus_connection_get_outgoing_unix_fds (DBusConnection *connection)
      * Gets the approximate number of uni fds of all messages in the outgoing message queue.
      */
    long getOutgoingFDs() {
        return dbus_connection_get_outgoing_unix_fds (ptr());
    }
     /**
      * DBUS_EXPORT dbus_bool_t dbus_connection_has_messages_to_send (DBusConnection *connection)
      */
    bool hasMessagesToSend() {
        return  dbus_connection_has_messages_to_send (ptr());
    }

    /////////////////////////
    // https://dbus.freedesktop.org/doc/api/html/group__DBusBus.html
     /**
      *  used in constructor
      *  DBusConnection * dbus_bus_get (DBusBusType type, DBusError *error)
      *      Connects to a bus daemon and registers the client with it.
      */

     /**:  used in constructor
      *  DBusConnection * dbus_bus_get_private (DBusBusType type, DBusError *error)
      *      Connects to a bus daemon and registers the client with it as with dbus_bus_register().
      */

     /**
      * dbus_bool_t dbus_bus_register (DBusConnection *connection, DBusError *error)
      *  Registers a connection with the bus.
      */
      bool registerBus() {
          Error error;
          dbus_bool_t ret = dbus_bus_register (ptr(), error.ptr());
          DBusException::check(error);
          return ret;
      }

     /**
      * dbus_bool_t dbus_bus_set_unique_name (DBusConnection *connection, const char *unique_name)
      *    Sets the unique name of the connection, as assigned by the message bus.
      */
    bool setUniqueName(const char * unique_name) {
        return dbus_bus_set_unique_name (ptr(), unique_name);
    }

     /**
      * const char * dbus_bus_get_unique_name (DBusConnection *connection)
      *    Gets the unique name of the connection as assigned by the message bus.
      */
    const char * getUniqName() {
        return dbus_bus_get_unique_name (ptr());
    }

     /**
      * unsigned long   dbus_bus_get_unix_user (DBusConnection *connection, const char *name, DBusError *error)
      * Asks the bus to return the UID the named connection authenticated as, if any.
      */
    unsigned long getUID(const char *name) {
        Error error;
        unsigned long uid = dbus_bus_get_unix_user (ptr(), name, error.ptr());
        DBusException::check(error);
        return uid;
    }

    /**
     * char *  dbus_bus_get_id (DBusConnection *connection, DBusError *error)
     * Asks the bus to return its globally unique ID, as described in the D-Bus specification.
     */
    char *  getID() {
        Error error;
        char * id = dbus_bus_get_id (ptr(), error.ptr());
        DBusException::check(error);
        return id;
    }

     /**
      * int dbus_bus_request_name (DBusConnection *connection,
      *       const char *name, unsigned int flags, DBusError *error)
      *  Asks the bus to assign the given name to this connection by invoking the RequestName method on the bus.
      **/
    int requestName(const char *name, unsigned int flags) {
        Error error;
        int ret = dbus_bus_request_name (ptr(), name, flags, error.ptr());
        DBusException::check(error);
        return ret;
    }

     /**
      * int  dbus_bus_release_name (DBusConnection *connection,
      *        const char *name, DBusError *error)
      *  Asks the bus to unassign the given name from this connection by invoking the ReleaseName method on the bus.
      */
    int releaseName(const char *name) {
        Error error;
        int ret = dbus_bus_release_name (ptr(), name, error.ptr());
        DBusException::check(error);
        return ret;
    }

     /**
      * dbus_bool_t  dbus_bus_name_has_owner (DBusConnection *connection,
      *            const char *name, DBusError *error)
      *  Asks the bus whether a certain name has an owner.
      */
    bool hasOwner(const char *name) {
        Error error;
        dbus_bool_t ret = dbus_bus_name_has_owner (ptr(), name, error.ptr());
        DBusException::check(error);
        return ret;
    }

     /**
      * dbus_bool_t dbus_bus_start_service_by_name (DBusConnection *connection,
      *          const char *name, dbus_uint32_t flags, dbus_uint32_t *result,
      *          DBusError *error)
      * Starts a service that will request ownership of the given name.
      */
    bool startService(const char *name, dbus_uint32_t flags, dbus_uint32_t & result) {
        Error error;
        dbus_bool_t ret = dbus_bus_start_service_by_name (ptr(), name, flags,
                &result, error.ptr());
        DBusException::check(error);
        return ret;
    }
     /**
      * void dbus_bus_add_match (DBusConnection *connection, const char *rule, DBusError *error)
      *  Adds a match rule to match messages going through the message bus.
      */
    void addMatch(const char *rule) {
        Error error;
        dbus_bus_add_match (ptr(),  rule, error.ptr());
        DBusException::check(error);
    }
     /**
      * void dbus_bus_remove_match (DBusConnection *connection,
      *               const char *rule, DBusError *error)
      * Removes a previously-added match rule "by value" (the most recently-added identical rule gets removed).
      **/
    void removeMatch(const char *rule) {
        Error error;
        dbus_bus_remove_match (ptr(),  rule, error.ptr());
        DBusException::check(error);
    }

protected:
    bool shared_;
};

}
}

#endif
