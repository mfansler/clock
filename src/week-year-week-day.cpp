#include "week-year-week-day.h"
#include "week-shim.h"
#include "calendar.h"
#include "duration.h"
#include "enums.h"
#include "get.h"
#include "rcrd.h"

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
new_year_week_day_from_fields(SEXP fields,
                              const cpp11::integers& precision_int,
                              SEXP start,
                              SEXP names) {
  const enum precision precision_val = parse_precision(precision_int);

  const r_ssize n_fields = Rf_xlength(fields);

  r_ssize n;
  switch (precision_val) {
  case precision::year: n = 1; break;
  case precision::week: n = 2; break;
  case precision::day: n = 3; break;
  case precision::hour: n = 4; break;
  case precision::minute: n = 5; break;
  case precision::second: n = 6; break;
  case precision::millisecond: n = 7; break;
  case precision::microsecond: n = 7; break;
  case precision::nanosecond: n = 7; break;
  default: never_reached("new_year_week_day_from_fields");
  }

  if (n != n_fields) {
    clock_abort("With the given precision, `fields` must have length %i, not %i.", n, n_fields);
  }

  SEXP out = PROTECT(new_clock_rcrd_from_fields(fields, names, classes_year_week_day));

  Rf_setAttrib(out, syms_precision, precision_int);
  Rf_setAttrib(out, syms_start, start);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
SEXP
year_week_day_restore(SEXP x, SEXP to) {
  SEXP precision = Rf_getAttrib(to, syms_precision);
  SEXP start = Rf_getAttrib(to, syms_start);

  SEXP out = PROTECT(clock_rcrd_restore(x, to, classes_year_week_day));

  Rf_setAttrib(out, syms_precision, precision);
  Rf_setAttrib(out, syms_start, start);

  UNPROTECT(1);
  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings
format_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                         const cpp11::integers& precision_int,
                         const cpp11::integers& start_int) {
  using namespace rclock;

  const week::start start = parse_week_start(start_int);

  cpp11::integers year = rweek::get_year(fields);
  cpp11::integers week = rweek::get_week(fields);
  cpp11::integers day = rweek::get_day(fields);
  cpp11::integers hour = rweek::get_hour(fields);
  cpp11::integers minute = rweek::get_minute(fields);
  cpp11::integers second = rweek::get_second(fields);
  cpp11::integers subsecond = rweek::get_subsecond(fields);

  rweek::y y{year, start};
  rweek::ywn ywn{year, week, start};
  rweek::ywnwd ywnwd{year, week, day, start};
  rweek::ywnwdh ywnwdh{year, week, day, hour, start};
  rweek::ywnwdhm ywnwdhm{year, week, day, hour, minute, start};
  rweek::ywnwdhms ywnwdhms{year, week, day, hour, minute, second, start};
  rweek::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
  case precision::year: return format_calendar_impl(y);
  case precision::week: return format_calendar_impl(ywn);
  case precision::day: return format_calendar_impl(ywnwd);
  case precision::hour: return format_calendar_impl(ywnwdh);
  case precision::minute: return format_calendar_impl(ywnwdhm);
  case precision::second: return format_calendar_impl(ywnwdhms);
  case precision::millisecond: return format_calendar_impl(ywnwdhmss1);
  case precision::microsecond: return format_calendar_impl(ywnwdhmss2);
  case precision::nanosecond: return format_calendar_impl(ywnwdhmss3);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("format_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
invalid_detect_year_week_day_cpp(const cpp11::integers& year,
                                 const cpp11::integers& week,
                                 const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  rclock::rweek::ywn x{year, week, start};

  const r_ssize size = x.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = false;
    } else {
      out[i] = !x.to_year_weeknum(i).ok();
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
bool
invalid_any_year_week_day_cpp(const cpp11::integers& year,
                              const cpp11::integers& week,
                              const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  rclock::rweek::ywn x{year, week, start};

  const r_ssize size = x.size();

  for (r_ssize i = 0; i < size; ++i) {
    if (!x.is_na(i) && !x.to_year_weeknum(i).ok()) {
      return true;
    }
  }

  return false;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
int
invalid_count_year_week_day_cpp(const cpp11::integers& year,
                                const cpp11::integers& week,
                                const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  rclock::rweek::ywn x{year, week, start};

  const r_ssize size = x.size();

  int count = 0;

  for (r_ssize i = 0; i < size; ++i) {
    count += !x.is_na(i) && !x.to_year_weeknum(i).ok();
  }

  return count;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
invalid_resolve_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                                  const cpp11::integers& precision_int,
                                  const cpp11::integers& start_int,
                                  const cpp11::strings& invalid_string) {
  using namespace rclock;

  const week::start start = parse_week_start(start_int);
  const enum invalid invalid_val = parse_invalid(invalid_string);

  cpp11::integers year = rweek::get_year(fields);
  cpp11::integers week = rweek::get_week(fields);
  cpp11::integers day = rweek::get_day(fields);
  cpp11::integers hour = rweek::get_hour(fields);
  cpp11::integers minute = rweek::get_minute(fields);
  cpp11::integers second = rweek::get_second(fields);
  cpp11::integers subsecond = rweek::get_subsecond(fields);

  rweek::ywn ywn{year, week, start};
  rweek::ywnwd ywnwd{year, week, day, start};
  rweek::ywnwdh ywnwdh{year, week, day, hour, start};
  rweek::ywnwdhm ywnwdhm{year, week, day, hour, minute, start};
  rweek::ywnwdhms ywnwdhms{year, week, day, hour, minute, second, start};
  rweek::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
  case precision::week: return invalid_resolve_calendar_impl(ywn, invalid_val);
  case precision::day: return invalid_resolve_calendar_impl(ywnwd, invalid_val);
  case precision::hour: return invalid_resolve_calendar_impl(ywnwdh, invalid_val);
  case precision::minute: return invalid_resolve_calendar_impl(ywnwdhm, invalid_val);
  case precision::second: return invalid_resolve_calendar_impl(ywnwdhms, invalid_val);
  case precision::millisecond: return invalid_resolve_calendar_impl(ywnwdhmss1, invalid_val);
  case precision::microsecond: return invalid_resolve_calendar_impl(ywnwdhmss2, invalid_val);
  case precision::nanosecond: return invalid_resolve_calendar_impl(ywnwdhmss3, invalid_val);
  default: never_reached("invalid_resolve_year_week_day_cpp");
  }
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::integers
get_year_week_day_last_cpp(const cpp11::integers& year,
                           const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  rclock::rweek::y x{year, start};

  const r_ssize size = x.size();
  cpp11::writable::integers out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out[i] = r_int_na;
    } else {
      const auto elt = x.to_year(i) / week::last;
      out[i] = static_cast<int>(static_cast<unsigned>(elt.weeknum()));
    }
  }

  return out;
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
year_week_day_plus_duration_cpp(cpp11::list_of<cpp11::integers> fields,
                                cpp11::list_of<cpp11::doubles> fields_n,
                                const cpp11::integers& precision_fields,
                                const cpp11::integers& precision_n,
                                const cpp11::integers& start_int) {
  using namespace rclock;

  const week::start start = parse_week_start(start_int);

  const enum precision precision_fields_val = parse_precision(precision_fields);
  const enum precision precision_n_val = parse_precision(precision_n);

  cpp11::integers year = rweek::get_year(fields);
  cpp11::integers week = rweek::get_week(fields);
  cpp11::integers day = rweek::get_day(fields);
  cpp11::integers hour = rweek::get_hour(fields);
  cpp11::integers minute = rweek::get_minute(fields);
  cpp11::integers second = rweek::get_second(fields);
  cpp11::integers subsecond = rweek::get_subsecond(fields);

  rweek::y y{year, start};
  rweek::ywn ywn{year, week, start};
  rweek::ywnwd ywnwd{year, week, day, start};
  rweek::ywnwdh ywnwdh{year, week, day, hour, start};
  rweek::ywnwdhm ywnwdhm{year, week, day, hour, minute, start};
  rweek::ywnwdhms ywnwdhms{year, week, day, hour, minute, second, start};
  rweek::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond, start};

  duration::years dy{fields_n};

  switch (precision_fields_val) {
  case precision::year:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(y, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::week:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywn, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::day:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwd, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::hour:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdh, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::minute:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhm, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::second:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhms, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::millisecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss1, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::microsecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss2, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  case precision::nanosecond:
    switch (precision_n_val) {
    case precision::year: return calendar_plus_duration_impl(ywnwdhmss3, dy);
    default: clock_abort("Internal error: Invalid precision.");
    }
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_week_day_plus_duration_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::list
as_sys_time_year_week_day_cpp(cpp11::list_of<cpp11::integers> fields,
                              const cpp11::integers& precision_int,
                              const cpp11::integers& start_int) {
  using namespace rclock;

  const week::start start = parse_week_start(start_int);

  cpp11::integers year = rweek::get_year(fields);
  cpp11::integers week = rweek::get_week(fields);
  cpp11::integers day = rweek::get_day(fields);
  cpp11::integers hour = rweek::get_hour(fields);
  cpp11::integers minute = rweek::get_minute(fields);
  cpp11::integers second = rweek::get_second(fields);
  cpp11::integers subsecond = rweek::get_subsecond(fields);

  rweek::ywnwd ywnwd{year, week, day, start};
  rweek::ywnwdh ywnwdh{year, week, day, hour, start};
  rweek::ywnwdhm ywnwdhm{year, week, day, hour, minute, start};
  rweek::ywnwdhms ywnwdhms{year, week, day, hour, minute, second, start};
  rweek::ywnwdhmss<std::chrono::milliseconds> ywnwdhmss1{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::microseconds> ywnwdhmss2{year, week, day, hour, minute, second, subsecond, start};
  rweek::ywnwdhmss<std::chrono::nanoseconds> ywnwdhmss3{year, week, day, hour, minute, second, subsecond, start};

  switch (parse_precision(precision_int)) {
  case precision::day: return as_sys_time_from_calendar_impl<duration::days>(ywnwd);
  case precision::hour: return as_sys_time_from_calendar_impl<duration::hours>(ywnwdh);
  case precision::minute: return as_sys_time_from_calendar_impl<duration::minutes>(ywnwdhm);
  case precision::second: return as_sys_time_from_calendar_impl<duration::seconds>(ywnwdhms);
  case precision::millisecond: return as_sys_time_from_calendar_impl<duration::milliseconds>(ywnwdhmss1);
  case precision::microsecond: return as_sys_time_from_calendar_impl<duration::microseconds>(ywnwdhmss2);
  case precision::nanosecond: return as_sys_time_from_calendar_impl<duration::nanoseconds>(ywnwdhmss3);
  default: {
    const enum precision precision_val = parse_precision(precision_int);
    const std::string precision_string = precision_to_cpp_string(precision_val);
    std::string message =
      "Can't convert to a time point from a calendar with '" +
      precision_string +
      "' precision. " +
      "A minimum of 'day' precision is required.";
    clock_abort(message.c_str());
  }
  }

  never_reached("as_sys_time_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

template <class ClockDuration, class Calendar>
cpp11::writable::list
as_year_week_day_from_sys_time_impl(cpp11::list_of<cpp11::doubles>& fields,
                                    week::start start) {
  using Duration = typename ClockDuration::chrono_duration;

  const ClockDuration x{fields};
  const r_ssize size = x.size();

  Calendar out(size, start);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i)) {
      out.assign_na(i);
    } else {
      const Duration elt = x[i];
      const date::sys_time<Duration> elt_st{elt};
      out.assign_sys_time(elt_st, i);
    }
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
as_year_week_day_from_sys_time_cpp(cpp11::list_of<cpp11::doubles> fields,
                                   const cpp11::integers& precision_int,
                                   const cpp11::integers& start_int) {
  using namespace rclock;

  const week::start start = parse_week_start(start_int);

  switch (parse_precision(precision_int)) {
  case precision::day: return as_year_week_day_from_sys_time_impl<duration::days, rweek::ywnwd>(fields, start);
  case precision::hour: return as_year_week_day_from_sys_time_impl<duration::hours, rweek::ywnwdh>(fields, start);
  case precision::minute: return as_year_week_day_from_sys_time_impl<duration::minutes, rweek::ywnwdhm>(fields, start);
  case precision::second: return as_year_week_day_from_sys_time_impl<duration::seconds, rweek::ywnwdhms>(fields, start);
  case precision::millisecond: return as_year_week_day_from_sys_time_impl<duration::milliseconds, rweek::ywnwdhmss<std::chrono::milliseconds>>(fields, start);
  case precision::microsecond: return as_year_week_day_from_sys_time_impl<duration::microseconds, rweek::ywnwdhmss<std::chrono::microseconds>>(fields, start);
  case precision::nanosecond: return as_year_week_day_from_sys_time_impl<duration::nanoseconds, rweek::ywnwdhmss<std::chrono::nanoseconds>>(fields, start);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("as_year_week_day_from_sys_time_cpp");
}

// -----------------------------------------------------------------------------

static
inline
cpp11::writable::list
year_minus_year_impl(const rclock::rweek::y& x,
                     const rclock::rweek::y& y) {
  const r_ssize size = x.size();
  rclock::duration::years out(size);

  for (r_ssize i = 0; i < size; ++i) {
    if (x.is_na(i) || y.is_na(i)) {
      out.assign_na(i);
      continue;
    }
    out.assign(x.to_year(i) - y.to_year(i), i);
  }

  return out.to_list();
}

[[cpp11::register]]
cpp11::writable::list
year_week_day_minus_year_week_day_cpp(cpp11::list_of<cpp11::integers> x,
                                      cpp11::list_of<cpp11::integers> y,
                                      const cpp11::integers& precision_int,
                                      const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  const cpp11::integers x_year = rclock::rweek::get_year(x);
  const cpp11::integers y_year = rclock::rweek::get_year(y);

  const rclock::rweek::y x_y{x_year, start};
  const rclock::rweek::y y_y{y_year, start};

  switch (parse_precision(precision_int)) {
  case precision::year: return year_minus_year_impl(x_y, y_y);
  default: clock_abort("Internal error: Invalid precision.");
  }

  never_reached("year_week_day_minus_year_week_day_cpp");
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::logicals
year_week_day_leap_year_cpp(const cpp11::integers& year,
                            const cpp11::integers& start_int) {
  const week::start start = parse_week_start(start_int);

  const r_ssize size = year.size();
  cpp11::writable::logicals out(size);

  for (r_ssize i = 0; i < size; ++i) {
    const int elt = year[i];

    if (elt == r_int_na) {
      out[i] = r_lgl_na;
    } else {
      out[i] = rclock::rweek::week_shim::year{elt, start}.is_leap();
    }
  }

  return out;
}
