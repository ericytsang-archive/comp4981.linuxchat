/**
 * defines the message structure used by the networking library
 *
 * @sourceFile Message.h
 *
 * @program    N/A
 *
 * @date       2015-03-23
 *
 * @revision   none
 *
 * @designer   Eric Tsang
 *
 * @programmer Eric Tsang
 *
 * @note       none
 */
#ifndef MESSAGE_H
#define MESSAGE_H

namespace Net
{
    /**
     * message structure used by the networking library.
     */
    struct Message
    {
        int type;   // user defined value, used to describe what is in payload
        void* data; // pointer to the data that is associated with the message
        int len;    // length of the data payload pointed to at the data pointer
    };
}

#endif
