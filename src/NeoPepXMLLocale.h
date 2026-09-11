// Scoped switch of the calling thread's numeric locale to "C".
//
// The writers format numbers with the printf family, which honors LC_NUMERIC. A host
// application running under a comma-decimal locale would otherwise get "0,9998" in its pepXML.
// The switch is thread-local on every supported platform, so other threads of the host are not
// affected, and the previous state is restored when the object goes out of scope.
//
// Private to the library; not installed.
#ifndef NEOPEPXMLLOCALE_H
#define NEOPEPXMLLOCALE_H

#include <clocale>
#include <locale.h>
#include <string>
#if defined(__APPLE__)
#include <xlocale.h>
#endif

class npxCNumericLocale {
public:
  npxCNumericLocale() {
#if defined(_WIN32)
    // Give this thread its own locale (initialized from the global one), then set its numeric
    // category. The global locale, and every other thread, stay as they were.
    previousMode_ = _configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
    const char* current = std::setlocale(LC_NUMERIC, nullptr);
    previous_ = current ? current : "C";
    std::setlocale(LC_NUMERIC, "C");
#else
    cLocale_ = newlocale(LC_NUMERIC_MASK, "C", (locale_t)0);
    previous_ = cLocale_ ? uselocale(cLocale_) : (locale_t)0;
#endif
  }

  ~npxCNumericLocale() {
#if defined(_WIN32)
    std::setlocale(LC_NUMERIC, previous_.c_str());
    if (previousMode_ != -1) _configthreadlocale(previousMode_);
#else
    if (cLocale_) {
      uselocale(previous_);
      freelocale(cLocale_);
    }
#endif
  }

  npxCNumericLocale(const npxCNumericLocale&) = delete;
  npxCNumericLocale& operator=(const npxCNumericLocale&) = delete;

private:
#if defined(_WIN32)
  int previousMode_;
  std::string previous_;
#else
  locale_t cLocale_;
  locale_t previous_;
#endif
};

#endif
