// Generated by cpp11: do not edit by hand
// clang-format off


#include "cpp11/declarations.hpp"

// adjust-zone.cpp
SEXP adjust_zone_retain_clock_cpp(SEXP x, SEXP zone, SEXP dst_nonexistent, SEXP dst_ambiguous);
extern "C" SEXP _civil_adjust_zone_retain_clock_cpp(SEXP x, SEXP zone, SEXP dst_nonexistent, SEXP dst_ambiguous) {
  BEGIN_CPP11
    return cpp11::as_sexp(adjust_zone_retain_clock_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(zone), cpp11::as_cpp<cpp11::decay_t<SEXP>>(dst_nonexistent), cpp11::as_cpp<cpp11::decay_t<SEXP>>(dst_ambiguous)));
  END_CPP11
}
// adjust.cpp
SEXP adjust_local_days_cpp(SEXP x, SEXP value, SEXP day_nonexistent, SEXP size, SEXP adjuster);
extern "C" SEXP _civil_adjust_local_days_cpp(SEXP x, SEXP value, SEXP day_nonexistent, SEXP size, SEXP adjuster) {
  BEGIN_CPP11
    return cpp11::as_sexp(adjust_local_days_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(value), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day_nonexistent), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size), cpp11::as_cpp<cpp11::decay_t<SEXP>>(adjuster)));
  END_CPP11
}
// adjust.cpp
SEXP adjust_local_time_of_day_cpp(SEXP x, SEXP value, SEXP size, SEXP adjuster);
extern "C" SEXP _civil_adjust_local_time_of_day_cpp(SEXP x, SEXP value, SEXP size, SEXP adjuster) {
  BEGIN_CPP11
    return cpp11::as_sexp(adjust_local_time_of_day_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(value), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size), cpp11::as_cpp<cpp11::decay_t<SEXP>>(adjuster)));
  END_CPP11
}
// adjust.cpp
SEXP adjust_local_nanos_of_second_cpp(SEXP x, SEXP value, SEXP size, SEXP adjuster);
extern "C" SEXP _civil_adjust_local_nanos_of_second_cpp(SEXP x, SEXP value, SEXP size, SEXP adjuster) {
  BEGIN_CPP11
    return cpp11::as_sexp(adjust_local_nanos_of_second_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(value), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size), cpp11::as_cpp<cpp11::decay_t<SEXP>>(adjuster)));
  END_CPP11
}
// arithmetic.cpp
SEXP add_years_or_months_cpp(SEXP x, SEXP n, SEXP day_nonexistent, SEXP unit, SEXP size);
extern "C" SEXP _civil_add_years_or_months_cpp(SEXP x, SEXP n, SEXP day_nonexistent, SEXP unit, SEXP size) {
  BEGIN_CPP11
    return cpp11::as_sexp(add_years_or_months_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(n), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day_nonexistent), cpp11::as_cpp<cpp11::decay_t<SEXP>>(unit), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size)));
  END_CPP11
}
// arithmetic.cpp
SEXP add_weeks_or_days_cpp(SEXP x, SEXP n, SEXP unit, SEXP size);
extern "C" SEXP _civil_add_weeks_or_days_cpp(SEXP x, SEXP n, SEXP unit, SEXP size) {
  BEGIN_CPP11
    return cpp11::as_sexp(add_weeks_or_days_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(n), cpp11::as_cpp<cpp11::decay_t<SEXP>>(unit), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size)));
  END_CPP11
}
// arithmetic.cpp
SEXP add_hours_or_minutes_or_seconds_local_cpp(SEXP x, SEXP n, SEXP unit, SEXP size);
extern "C" SEXP _civil_add_hours_or_minutes_or_seconds_local_cpp(SEXP x, SEXP n, SEXP unit, SEXP size) {
  BEGIN_CPP11
    return cpp11::as_sexp(add_hours_or_minutes_or_seconds_local_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(n), cpp11::as_cpp<cpp11::decay_t<SEXP>>(unit), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size)));
  END_CPP11
}
// arithmetic.cpp
SEXP add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(SEXP x, SEXP n, SEXP unit, SEXP size);
extern "C" SEXP _civil_add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(SEXP x, SEXP n, SEXP unit, SEXP size) {
  BEGIN_CPP11
    return cpp11::as_sexp(add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(x), cpp11::as_cpp<cpp11::decay_t<SEXP>>(n), cpp11::as_cpp<cpp11::decay_t<SEXP>>(unit), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size)));
  END_CPP11
}
// converters.cpp
SEXP convert_seconds_to_days_and_time_of_day_cpp(SEXP seconds, SEXP zone);
extern "C" SEXP _civil_convert_seconds_to_days_and_time_of_day_cpp(SEXP seconds, SEXP zone) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_seconds_to_days_and_time_of_day_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(seconds), cpp11::as_cpp<cpp11::decay_t<SEXP>>(zone)));
  END_CPP11
}
// converters.cpp
SEXP convert_days_and_time_of_day_to_seconds_cpp(SEXP days, SEXP time_of_day, SEXP zone, SEXP dst_nonexistent, SEXP dst_ambiguous, SEXP size);
extern "C" SEXP _civil_convert_days_and_time_of_day_to_seconds_cpp(SEXP days, SEXP time_of_day, SEXP zone, SEXP dst_nonexistent, SEXP dst_ambiguous, SEXP size) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_days_and_time_of_day_to_seconds_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(days), cpp11::as_cpp<cpp11::decay_t<SEXP>>(time_of_day), cpp11::as_cpp<cpp11::decay_t<SEXP>>(zone), cpp11::as_cpp<cpp11::decay_t<SEXP>>(dst_nonexistent), cpp11::as_cpp<cpp11::decay_t<SEXP>>(dst_ambiguous), cpp11::as_cpp<cpp11::decay_t<SEXP>>(size)));
  END_CPP11
}
// converters.cpp
SEXP convert_year_month_day_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP day_nonexistent);
extern "C" SEXP _civil_convert_year_month_day_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP day_nonexistent) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_year_month_day_to_fields_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(year), cpp11::as_cpp<cpp11::decay_t<SEXP>>(month), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day_nonexistent)));
  END_CPP11
}
// converters.cpp
SEXP convert_year_month_day_hour_minute_second_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP hour, SEXP minute, SEXP second, SEXP day_nonexistent);
extern "C" SEXP _civil_convert_year_month_day_hour_minute_second_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP hour, SEXP minute, SEXP second, SEXP day_nonexistent) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_year_month_day_hour_minute_second_to_fields_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(year), cpp11::as_cpp<cpp11::decay_t<SEXP>>(month), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day), cpp11::as_cpp<cpp11::decay_t<SEXP>>(hour), cpp11::as_cpp<cpp11::decay_t<SEXP>>(minute), cpp11::as_cpp<cpp11::decay_t<SEXP>>(second), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day_nonexistent)));
  END_CPP11
}
// converters.cpp
SEXP convert_year_month_day_hour_minute_second_nanos_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP hour, SEXP minute, SEXP second, SEXP nanos, SEXP day_nonexistent);
extern "C" SEXP _civil_convert_year_month_day_hour_minute_second_nanos_to_fields_cpp(SEXP year, SEXP month, SEXP day, SEXP hour, SEXP minute, SEXP second, SEXP nanos, SEXP day_nonexistent) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_year_month_day_hour_minute_second_nanos_to_fields_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(year), cpp11::as_cpp<cpp11::decay_t<SEXP>>(month), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day), cpp11::as_cpp<cpp11::decay_t<SEXP>>(hour), cpp11::as_cpp<cpp11::decay_t<SEXP>>(minute), cpp11::as_cpp<cpp11::decay_t<SEXP>>(second), cpp11::as_cpp<cpp11::decay_t<SEXP>>(nanos), cpp11::as_cpp<cpp11::decay_t<SEXP>>(day_nonexistent)));
  END_CPP11
}
// converters.cpp
SEXP convert_days_to_year_month_day_cpp(SEXP days);
extern "C" SEXP _civil_convert_days_to_year_month_day_cpp(SEXP days) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_days_to_year_month_day_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(days)));
  END_CPP11
}
// converters.cpp
SEXP convert_time_of_day_to_hour_minute_second_cpp(SEXP time_of_day);
extern "C" SEXP _civil_convert_time_of_day_to_hour_minute_second_cpp(SEXP time_of_day) {
  BEGIN_CPP11
    return cpp11::as_sexp(convert_time_of_day_to_hour_minute_second_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(time_of_day)));
  END_CPP11
}
// floor.cpp
SEXP floor_days_to_year_month_cpp(SEXP days);
extern "C" SEXP _civil_floor_days_to_year_month_cpp(SEXP days) {
  BEGIN_CPP11
    return cpp11::as_sexp(floor_days_to_year_month_cpp(cpp11::as_cpp<cpp11::decay_t<SEXP>>(days)));
  END_CPP11
}
// initialize.cpp
SEXP civil_init();
extern "C" SEXP _civil_civil_init() {
  BEGIN_CPP11
    return cpp11::as_sexp(civil_init());
  END_CPP11
}
// install.cpp
void civil_set_install(SEXP path);
extern "C" SEXP _civil_civil_set_install(SEXP path) {
  BEGIN_CPP11
    civil_set_install(cpp11::as_cpp<cpp11::decay_t<SEXP>>(path));
    return R_NilValue;
  END_CPP11
}
// zone.cpp
SEXP zone_standardize(SEXP zone);
extern "C" SEXP _civil_zone_standardize(SEXP zone) {
  BEGIN_CPP11
    return cpp11::as_sexp(zone_standardize(cpp11::as_cpp<cpp11::decay_t<SEXP>>(zone)));
  END_CPP11
}
// zone.cpp
SEXP zone_is_valid(SEXP zone);
extern "C" SEXP _civil_zone_is_valid(SEXP zone) {
  BEGIN_CPP11
    return cpp11::as_sexp(zone_is_valid(cpp11::as_cpp<cpp11::decay_t<SEXP>>(zone)));
  END_CPP11
}

extern "C" {
/* .Call calls */
extern SEXP _civil_add_hours_or_minutes_or_seconds_local_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_add_milliseconds_or_microseconds_or_nanoseconds_local_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_add_weeks_or_days_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_add_years_or_months_cpp(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_adjust_local_days_cpp(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_adjust_local_nanos_of_second_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_adjust_local_time_of_day_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_adjust_zone_retain_clock_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_civil_init();
extern SEXP _civil_civil_set_install(SEXP);
extern SEXP _civil_convert_days_and_time_of_day_to_seconds_cpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_convert_days_to_year_month_day_cpp(SEXP);
extern SEXP _civil_convert_seconds_to_days_and_time_of_day_cpp(SEXP, SEXP);
extern SEXP _civil_convert_time_of_day_to_hour_minute_second_cpp(SEXP);
extern SEXP _civil_convert_year_month_day_hour_minute_second_nanos_to_fields_cpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_convert_year_month_day_hour_minute_second_to_fields_cpp(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_convert_year_month_day_to_fields_cpp(SEXP, SEXP, SEXP, SEXP);
extern SEXP _civil_floor_days_to_year_month_cpp(SEXP);
extern SEXP _civil_zone_is_valid(SEXP);
extern SEXP _civil_zone_standardize(SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"_civil_add_hours_or_minutes_or_seconds_local_cpp",                     (DL_FUNC) &_civil_add_hours_or_minutes_or_seconds_local_cpp,                     4},
    {"_civil_add_milliseconds_or_microseconds_or_nanoseconds_local_cpp",     (DL_FUNC) &_civil_add_milliseconds_or_microseconds_or_nanoseconds_local_cpp,     4},
    {"_civil_add_weeks_or_days_cpp",                                         (DL_FUNC) &_civil_add_weeks_or_days_cpp,                                         4},
    {"_civil_add_years_or_months_cpp",                                       (DL_FUNC) &_civil_add_years_or_months_cpp,                                       5},
    {"_civil_adjust_local_days_cpp",                                         (DL_FUNC) &_civil_adjust_local_days_cpp,                                         5},
    {"_civil_adjust_local_nanos_of_second_cpp",                              (DL_FUNC) &_civil_adjust_local_nanos_of_second_cpp,                              4},
    {"_civil_adjust_local_time_of_day_cpp",                                  (DL_FUNC) &_civil_adjust_local_time_of_day_cpp,                                  4},
    {"_civil_adjust_zone_retain_clock_cpp",                                  (DL_FUNC) &_civil_adjust_zone_retain_clock_cpp,                                  4},
    {"_civil_civil_init",                                                    (DL_FUNC) &_civil_civil_init,                                                    0},
    {"_civil_civil_set_install",                                             (DL_FUNC) &_civil_civil_set_install,                                             1},
    {"_civil_convert_days_and_time_of_day_to_seconds_cpp",                   (DL_FUNC) &_civil_convert_days_and_time_of_day_to_seconds_cpp,                   6},
    {"_civil_convert_days_to_year_month_day_cpp",                            (DL_FUNC) &_civil_convert_days_to_year_month_day_cpp,                            1},
    {"_civil_convert_seconds_to_days_and_time_of_day_cpp",                   (DL_FUNC) &_civil_convert_seconds_to_days_and_time_of_day_cpp,                   2},
    {"_civil_convert_time_of_day_to_hour_minute_second_cpp",                 (DL_FUNC) &_civil_convert_time_of_day_to_hour_minute_second_cpp,                 1},
    {"_civil_convert_year_month_day_hour_minute_second_nanos_to_fields_cpp", (DL_FUNC) &_civil_convert_year_month_day_hour_minute_second_nanos_to_fields_cpp, 8},
    {"_civil_convert_year_month_day_hour_minute_second_to_fields_cpp",       (DL_FUNC) &_civil_convert_year_month_day_hour_minute_second_to_fields_cpp,       7},
    {"_civil_convert_year_month_day_to_fields_cpp",                          (DL_FUNC) &_civil_convert_year_month_day_to_fields_cpp,                          4},
    {"_civil_floor_days_to_year_month_cpp",                                  (DL_FUNC) &_civil_floor_days_to_year_month_cpp,                                  1},
    {"_civil_zone_is_valid",                                                 (DL_FUNC) &_civil_zone_is_valid,                                                 1},
    {"_civil_zone_standardize",                                              (DL_FUNC) &_civil_zone_standardize,                                              1},
    {NULL, NULL, 0}
};
}

extern "C" void R_init_civil(DllInfo* dll){
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
