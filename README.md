# Open-Banapass

## About this Project
An open-source Banapassport API DLL replacement for Wangan Midnight Maximum Tune 6.
This software is based on the original version written by [Luna](https://github.com/ry00001) for project Asakura, which can be found [here](https://github.com/ProjectAsakura/Bayshore).

This software has been rewritten to utilise a different card file format 
(`.txt` as opposed to `.ini`), which is formatted as follows:

`cardchipid;accesscode`

With an example file having the following contents:

`22411386745001192320;ECC153683A184D9DA3A460683B79426E`

Please note, these files do not include a newline character at the end. If you do not
provide a `backup.card.txt` file, a random card file will be created automatically. 
Both `card.txt` and `backup.card.txt` will be created in and read from the same root 
directory as the compiled `bngrw.dll` file.

### Card Reader Support

This project was originally created to be compatible with another project, [Py-Banapass](https://github.com/dragapult-xyz/py-banapass). This software 
acts as a middleware between an NFC card reader (Compatible with py-nfc) and
the custom dll, writing a card id and access code generated using the tapped
card to a filename `card.txt` which is then processed by the open-banapass.

After reading, the `card.txt` contents will be cleared until another card is
tapped, and more data is written. 

### Button Press Support

This software still supports the traditional method of tapping 'C' on the keyboard
to trigger sending the card data. The card data sent is read from the `backup.card.txt`
file, which will be randomly generated if it does not exist.

### Advanced Settings

There are a number of different viables which can be changed in the software, if
you would like to recompile the application yourself. These are as follows:

| File | Name | Default | Purpose |
| ---- | ---- | ------- | ------- | 
| `banapass.cpp` | `BANA_API_VERSION` | `Ver 1.6.0` | Specifies the Banapassport API Version |
| `banapass.cpp` | `CARD_FILENAME` | `card.txt` | Card filename which is monitored for changes |
| `banapass.cpp` | `BACKUP_FILENAME` | `backup.card.txt` | Card filename to use if `BACKUP_KEY` is pressed |
| `banapass.cpp` | `BACKUP_KEY` | `C` | Key which is pressed to use backup card |
| `banapass.cpp` | `DELIM_KEY` | `;` | Delimiter character for the card files |
| `banapass.cpp` | `WAIT_TOUCH_TIMEOUT` | `250` | Time (in milliseconds) the card reader thread will wait before checking again |
| `log.cpp` | `LOG_FILE` | `bngrw.log` | If using the debug version of the DLL, this file will be written to when events occur. If this value is deleted, no file logging will occur. This value is not defined in the release version of the DLL. |

## Special thanks
 - [Luna](https://github.com/ry00001) - Original version
 - [derole](https://derole.co.uk) - Code, technical help
 - [PockyWitch](https://github.com/EmiMidnight) - Advice, technical help

## Changelog

Please see below for the change history for the software.

## Ver. 2.0.0

Cleaned up repository, updated readme file, worked on
fixing issue with some games where the card data is 
overwritten before it can be sent to the server

It is believed that this issue has been resolved, however 
more testing will be required to be conclusive

## Ver. 1.1.0
Cleaned up constants, now compiling in release mode

## Ver. 1.0.1
Added comments

## Ver. 1.0.0
Cloned from source repository, added support for two different
card files, py-banapass support implemented