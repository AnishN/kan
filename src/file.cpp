#include "file.hpp"

Error File::init(String *path, FileMode mode) {
    Error error = Error::Okay;
    try_error(this->path.init_from_string(path));
    FILE *handle = NULL;
    switch(mode) {
        case FileMode::RO:
            handle = fopen(path->get_ptr(), "r");
            break;
        case FileMode::WO:
            handle = fopen(path->get_ptr(), "w");
            break;
        case FileMode::RW:
            handle = fopen(path->get_ptr(), "rw");
            break;
    }
    try_ptr(handle, Error::InvalidPath);
    this->handle = handle;
    this->mode = mode;
    return error;
}

void File::fini() {
    fclose(this->handle);
    this->path.fini();
    this->handle = NULL;
    this->mode = (FileMode)0;
}

uint64_t File::get_size() {
    fseek(this->handle, 0L, SEEK_END);
    uint64_t file_size = ftell(this->handle);
    fseek(this->handle, 0L, SEEK_SET);
    return file_size;
}

bool File::is_at_end() {
    if(feof(this->handle) == 0) {
        return false;
    }
    return true;
}

Error File::read_string(String *string) {
    string->fini();
    uint64_t file_size =  this->get_size();
    try_error(string->init_from_len(file_size));
    uint64_t read_size = fread(
        (char *)string->get_ptr(), file_size, sizeof(uint8_t), this->handle);
    if(read_size != file_size) {
        if(this->is_at_end()) {
            
        }
    }
    return Error::Okay;
}