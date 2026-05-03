#ifndef ALPHA_CONSTANTS_H
#define ALPHA_CONSTANTS_H

// Command Codes
#define WRITE_TEXT            'A'  // Write TEXT file
#define READ_TEXT             'B'  // Read TEXT file
#define WRITE_SPECIAL         'E'  // Write SPECIAL FUNCTION commands
#define READ_SPECIAL          'F'  // Read SPECIAL FUNCTION commands
#define WRITE_STRING          'G'  // Write STRING
#define READ_STRING           'H'  // Read STRING
#define WRITE_SMALL_DOTS      'I'  // Write SMALL DOTS PICTURE file
#define READ_SMALL_DOTS       'J'  // Read SMALL DOTS PICTURE file
#define WRITE_RGB_DOTS        'K'  // Write RGB DOTS PICTURE file
#define READ_RGB_DOTS         'L'  // Read RGB DOTS PICTURE file
#define WRITE_LARGE_DOTS      'M'  // Write LARGE DOTS PICTURE file
#define READ_LARGE_DOTS       'N'  // Read LARGE DOTS PICTURE file
#define WRITE_ALPHAVISION     'O'  // Write ALPHAVISION BULLETIN
#define SET_TIMEOUT           'T'  // Set Timeout Message

#define FILE_UNLOCKED         'U'
#define FILE_LOCKED           'L'

// File Types
#define FILE_TYPE_TEXT        'A'
#define FILE_TYPE_STRING      'B'

// Constants used in transmission packets
#define NUL                   0x00  // NULL
#define SOH                   0x01  // Start of Header
#define STX                   0x02  // Start of TeXt
#define ETX                   0x03  // End of TeXt
#define EOT                   0x04  // End Of Transmission
#define BEL                   0x07  // Bell
#define BS                    0x08  // Backspace
#define HT                    0x09  // Horizontal tab
#define LF                    0x0A  // Line Feed
#define NL                    0x0A  // New Line
#define VT                    0x0B  // Vertical Tab
#define NP                    0x0C  // New Page
#define CR                    0x0D  // Carriage Return
#define CAN                   0x18  // Cancel
#define SUB                   0x1A  // Substitute (select charset)
#define ESC                   0x1B  // Escape character

#define NEWLINE               NL
#define NEWPAGE               NP

// Default packet settings
#define DEFAULT_TYPE          'Z'
#define DEFAULT_ADDRESS       "00"

#endif // ALPHA_CONSTANTS_H
