module;

#include <stdexcept>
#include <string> // TODO: Circular import will occur if we use mystl::String

export module exception;

export namespace mystl {

// Base exception class for all project-specific exceptions
class BaseException : public std::exception {
public:
  explicit BaseException(const std::string &message) : message_(message) {}

  virtual const char *what() const noexcept override {
    return message_.c_str();
  }

private:
  std::string message_;
};

// Memory-related exceptions
class OutOfMemoryException : public BaseException {
public:
  explicit OutOfMemoryException(const std::string &message = "Out of memory")
      : BaseException(message) {}
};

class InvalidMemoryAccessException : public BaseException {
public:
  explicit InvalidMemoryAccessException(
      const std::string &message = "Invalid memory access")
      : BaseException(message) {}
};

// Range-related exceptions
class IndexOutOfRangeException : public BaseException {
public:
  explicit IndexOutOfRangeException(
      const std::string &message = "Index out of range")
      : BaseException(message) {}
};

class EmptyContainerException : public BaseException {
public:
  explicit EmptyContainerException(
      const std::string &message = "Operation on empty container")
      : BaseException(message) {}
};

// Invalid argument exceptions
class InvalidArgumentException : public BaseException {
public:
  explicit InvalidArgumentException(
      const std::string &message = "Invalid argument")
      : BaseException(message) {}
};

class NullPointerException : public BaseException {
public:
  explicit NullPointerException(
      const std::string &message = "Null pointer access")
      : BaseException(message) {}
};

// IO-related exceptions
class IOException : public BaseException {
public:
  explicit IOException(const std::string &message = "IO error")
      : BaseException(message) {}
};

class FileNotFoundException : public IOException {
public:
  explicit FileNotFoundException(const std::string &message = "File not found")
      : IOException(message) {}
};

class FileAccessException : public IOException {
public:
  explicit FileAccessException(const std::string &message = "File access error")
      : IOException(message) {}
};

// Algorithm-related exceptions
class UnsupportedOperationException : public BaseException {
public:
  explicit UnsupportedOperationException(
      const std::string &message = "Operation not supported")
      : BaseException(message) {}
};

class OverflowException : public BaseException {
public:
  explicit OverflowException(const std::string &message = "Overflow error")
      : BaseException(message) {}
};

class UnderflowException : public BaseException {
public:
  explicit UnderflowException(const std::string &message = "Underflow error")
      : BaseException(message) {}
};

} // namespace mystl
