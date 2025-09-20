import os
from .path_utils import GetAbsoluteTemporaryDir, CreateRecursiveDirIfNotExists


def _GetCacheFilePath(filePath: str) -> str:
    """
    Helper function to get the path of the cache file for a given file.
    """
    dirPath = os.path.dirname(filePath)
    fileName = os.path.basename(filePath)
    tmpDir = GetAbsoluteTemporaryDir()

    CreateRecursiveDirIfNotExists(dirPath)

    return os.path.join(tmpDir, dirPath, f".{fileName}.stamp")


def IsFileModified(filePath: str) -> bool:
    """
    Used for checking whether a file has been changed since the last time it was cached.

    Parameters
    ----------
    filePath : str
        The path to the file to check (relative to the `ABSOLUTE_BASE_DIR`).

    Returns
    -------
    bool
        True if the file has been modified since the last cache, False otherwise.
        If the file has never been cached, it is considered modified and will return True.
    """
    if not os.path.exists(filePath):
        return False

    if not os.path.exists(_GetCacheFilePath(filePath)):
        return True

    if os.path.getmtime(filePath) > os.path.getmtime(_GetCacheFilePath(filePath)):
        return True

    return False


def UpdateFileCache(filePath: str) -> None:
    """
    Updates the cache for a given file to mark it as unmodified.

    Parameters
    ----------
    filePath : str
        The path to the file to update the cache for (relative to the `ABSOLUTE_BASE_DIR`).
        If the file has never been cached, it will be added to the cache. If it has been cached before,
        its cache entry will be updated to reflect the current state of the file. This function does not return any value.
    """

    with open(_GetCacheFilePath(filePath), "w") as f:
        f.write("")
