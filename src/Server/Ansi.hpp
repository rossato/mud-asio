#ifndef ANSI_HPP
#define ANSI_HPP

// These are technically telnet sequences

#define ECHOON "\xff\xfc\x01"
#define ECHOOFF "\xff\xfb\x01"

#define NEWLINE "\r\n"

// Ansi sequences

#define BOLDTEXT "\x1b[1m"
#define PLAINTEXT "\x1b[0m"
#define GREENTEXT "\x1b[32m"
#define MAGENTATEXT "\x1b[35m"

#endif
