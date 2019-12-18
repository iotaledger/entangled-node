const fs = require('fs');

/** 
 * -----------------------------------------------------------------------------
 * Renames all occurences of "helpers/digest." to "helpers/digestx."
 * Renames file common/helpers/digest.c  to common/helpers/digestx.c
 * Renames common/helpers/digest.h to common/helpers/digestx.h
 * Renames all occurences of "common/trinary/ptrit." to "common/trinary/ptritx."
 * Renames file common/trinary/ptrit.c to common/trinary/ptritx.c
 * Renames file common/helpers/ptrit.h to common/helpers/ptritx.h
 * -----------------------------------------------------------------------------
 */

/**
 * Promisified version of fs.readdir
 * 
 * @method readdirAsync
 * 
 * @param {string} path
 * 
 * @returns {Promise} 
 */
const readdirAsync = (path) => new Promise((resolve, reject) => {
    return fs.readdir(path, (err, filenames) => err ? reject(err) : resolve(filenames))
});

/**
 * Promisified version of fs.rename
 * 
 * @method renameAsync
 * 
 * @param {string} oldPath
 * @param {string} newPath
 * 
 * @returns {Promise} 
 */
const renameAsync = (oldPath, newPath) => new Promise((resolve, reject) => {
    return fs.rename(oldPath, newPath, (err) => err ? reject(err) : resolve())
});

/**
 * Promisified version of fs.readFile
 * 
 * @method readFileAsync
 * 
 * @param {string} path
 * @param {string} [encoding]
 * 
 * @returns {Promise} 
 */
const readFileAsync = (path, encoding = 'utf-8') => new Promise((resolve, reject) => {
    return fs.readFile(path, encoding, (err, content) => err ? reject(err) : resolve(content))
});

/**
 * Promisified version of fs.writeFile
 * 
 * @method writeFileAsync
 * 
 * @param {string} path 
 * @param {string} content 
 * 
 * @returns {Promise}
 */
const writeFileAsync = (path, content) => new Promise((resolve, reject) => {
    return fs.writeFile(path, content, (err) => err ? reject(err) : resolve())
});

/**
 * Directories to ignore
 */
const BLACKLISTED_DIRECTORIES = [
    '.git',
    'node_modules',
];

/**
 * Files to ignore
 */
const BLACKLISTTED_FILES = [
    __filename,
    '.txt',
    '.pdf'
];

/**
 * Files to rename
 */
const FILES_TO_RENAME = [
    {
        old: 'common/trinary/ptrit.c',
        new: 'common/trinary/ptritx.c'
    },
    {
        old: 'common/trinary/ptrit.h',
        new: 'common/trinary/ptritx.h'
    },
    {
        old: 'common/helpers/digest.c',
        new: 'common/helpers/digestx.c'
    }, {
        old: 'common/helpers/digest.h',
        new: 'common/helpers/digestx.h'
    }
];

/**
 * Returns a (flattened) list of all file names in the provided directory
 * 
 * @method getFilePaths
 * 
 * @param {string} dirname 
 * 
 * @returns {Promise}
 */
const getFilePaths = (dirname) => {
    const _get = (_dirname, files = [], directories = []) => {
        return readdirAsync(_dirname).then((filenames) => {
            filenames.forEach((filename) => {
                const path = `${_dirname}/${filename}`;
                if (fs.lstatSync(path).isDirectory()) {
                    if (!BLACKLISTED_DIRECTORIES.some((directory) => directory.endsWith(filename))) {
                        directories.push(path);
                    }
                } else {
                    if (!BLACKLISTTED_FILES.some((_filename) => filename === _filename || _filename.endsWith(filename))) {
                        files.push(path);
                    }
                }
            });
            if (directories.length) {
                return directories.reduce((promise, directoryPath) => (
                    promise.then(() => {
                        const index = directories.indexOf(directoryPath);
                        if (index !== -1) {
                            directories.splice(index, 1);
                        }
                        return _get(directoryPath, files, directories);
                    })
                ), Promise.resolve());
            }
            return files;
        });
    };
    return _get(dirname);
};

/** Current platform */
const __PLATFORM__ = process.platform;

const isWindows = __PLATFORM__ === 'win32';


if (isWindows) {
    getFilePaths(process.cwd()).then((filenames) => {
        const paths = filenames.filter((filename, index) => {
            return filenames.indexOf(filename) == index;
        });

        return paths.reduce((promise, path, index) => (
            promise.then(() => {
                console.log(`Processing ${index + 1} of ${paths.length} files`);

                return readFileAsync(path).then((content) => {
                    let updatedContent = content.replace(new RegExp('helpers\/digest\\.'), 'helpers/digestx.');
                    updatedContent = updatedContent.replace(new RegExp('common\/trinary\/ptrit\\.'), 'common/trinary/ptritx.');

                    const hasContentChanged = content !== updatedContent;

                    return (hasContentChanged ? writeFileAsync(path, updatedContent) : Promise.resolve()).then(() => {
                        const fileToRename = FILES_TO_RENAME.find((object) => path.includes(object.old));

                        if (fileToRename) {
                            const newPath = path.split(fileToRename.old).join(fileToRename.new);
                            return renameAsync(path, newPath);
                        }
                    });
                });
            })
        ), Promise.resolve());
    }).then(() => console.info('Successfully patched.')).catch(console.error);
} else {
    console.info(`Did not run the patch. The current platform is ${__PLATFORM__}. This script is only meant to run on Windows.`);
}
