#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// ============================
// Includes
// ============================
#include <stddef.h>

// ============================
// Macros and Constants
// ============================

// ============================
// Type Definitions
// ============================
/**
 * @brief Handle for file operations
 */
typedef int pal_fs_file_t;

/**
 * @brief File open modes
 */
typedef enum pal_fs_open_mode_e
{
	PAL_FS_OPEN_MODE_READ,		  //!< Open file for reading. Does not create if not exists.
	PAL_FS_OPEN_MODE_WRITE,		  //!< Open file for writing. Create if not exists.
	PAL_FS_OPEN_MODE_APPEND,	  //!< Open file for appending. Create if not exists.
	PAL_FS_OPEN_MODE_READ_WRITE,  //!< Open file for reading and writing.
} pal_fs_open_mode_t;

// ============================
// Function Declarations
// ============================
/**
 * @brief Initialize the file system.
 * @return 0 on success, -1 on failure.
 * @note This function must be called before any file operations.
 */
int pal_fs_init(void);

/**
 * @brief Open a file.
 * @param path Path to the file.
 * @param mode Mode to open the file in. Refer to pal_fs_open_mode_t for options.
 * @return Handle to the opened file, or -1 on failure.
 * @note The file must be closed using pal_fs_close() after use.
 */
pal_fs_file_t pal_fs_open(const char *path, pal_fs_open_mode_t mode);

/**
 * @brief Write data to a file.
 * @param file Handle to the opened file.
 * @param buffer Pointer to the data to write.
 * @param size Size of the data to write in bytes.
 * @return Number of bytes written, or -1 on failure.
 * @note The file must be opened in write or append mode.
 */
int pal_fs_write(pal_fs_file_t file, const void *buffer, size_t size);

/**
 * @brief Read data from a file.
 * @param file Handle to the opened file.
 * @param buffer Pointer to the buffer to store the read data.
 * @param size Size of the buffer in bytes.
 * @return Number of bytes read, or -1 on failure.
 * @note The file must be opened in read or read/write mode.
 */
int pal_fs_read(pal_fs_file_t file, void *buffer, size_t size);

/**
 * @brief Seek to a specific position in a file.
 * @param file Handle to the opened file.
 * @param offset Offset from the beginning.
 * @return 0 on success, -1 on failure.
 */
int pal_fs_seek(pal_fs_file_t file, int offset);

/**
 * @brief Remove a file.
 * @param path Path to the file to remove.
 * @return 0 on success, -1 on failure.
 * @note This function does not check if the file is open. It is the caller's responsibility to close the file before removing it.
 */
int pal_fs_remove(const char *path);

/**
 * @brief Get the size of a file.
 * @param path Path to the file.
 * @return Size of the file in bytes, or -1 on failure.
 */
size_t pal_fs_get_size(const char *path);

/**
 * @brief Close a previously opened file.
 * @param file Handle to the opened file.
 * @return 0 on success, -1 on failure.
 */
int pal_fs_close(pal_fs_file_t file);

/**
 * @brief Deinitialize the file system.
 * @return 0 on success, -1 on failure.
 * @note This function will fail if there are still files in the file system.
 */
int pal_fs_deinit(void);

#ifdef __cplusplus
}
#endif
