#include "clock.h"
#include "utils.h"
#include "enums.h"
#include "conversion.h"
#include "resolve.h"
#include "clock-rcrd.h"
#include "check.h"
#include "zone.h"
#include <sstream>
#include <locale>

/*
 * Reference for format tokens
 * https://howardhinnant.github.io/date/date.html#to_stream_formatting
 */

// -----------------------------------------------------------------------------

static
inline
std::pair<const std::string*, const std::string*>
fill_weekday_names(const cpp11::strings& day,
                   const cpp11::strings& day_ab,
                   std::string (&weekday_names)[14]) {
  for (int i = 0; i < 7; ++i) {
    std::string string = day[i];
    weekday_names[i] = string;
  }
  for (int i = 0; i < 7; ++i) {
    weekday_names[i + 7] = day_ab[i];
  }
  return std::make_pair(weekday_names, weekday_names+sizeof(weekday_names)/sizeof(weekday_names[0]));
}

static
inline
std::pair<const std::string*, const std::string*>
fill_month_names(const cpp11::strings& mon,
                 const cpp11::strings& mon_ab,
                 std::string (&month_names)[24]) {
  for (int i = 0; i < 12; ++i) {
    std::string string = mon[i];
    month_names[i] = string;
  }
  for (int i = 0; i < 12; ++i) {
    month_names[i + 12] = mon_ab[i];
  }
  return std::make_pair(month_names, month_names+sizeof(month_names)/sizeof(month_names[0]));
}

static
inline
std::pair<const std::string*, const std::string*>
fill_ampm_names(const cpp11::strings& am_pm, std::string (&ampm_names)[2]) {
  for (int i = 0; i < 2; ++i) {
    std::string string = am_pm[i];
    ampm_names[i] = string;
  }
  return std::make_pair(ampm_names, ampm_names+sizeof(ampm_names)/sizeof(ampm_names[0]));
}

// -----------------------------------------------------------------------------

/*
 * Custom seconds formatter to be able to use user-supplied decimal mark.
 * Essentially copies the `decimal_format_seconds` `<<` operator.
 * Requires a bit of a roundabout hack to construct a `decimal_format_seconds`
 * object, as you can't just extract it from `tod`.
 */
template <class CharT, class Traits, class Duration>
static
inline
std::basic_ostream<CharT, Traits>&
stream_seconds(std::basic_ostream<CharT, Traits>& os,
               const date::hh_mm_ss<Duration>& tod,
               const CharT* decimal_mark)
{
  using date::detail::save_ostream;
  // NOTE: Custom formatting here to use user-supplied `decimal_mark`
  save_ostream<CharT, Traits> _(os);
  os.fill('0');
  os.width(2);
  os.flags(std::ios::dec | std::ios::right);
  // TODO: This is a hack to get the private `s_` out from `tod`. Anything better?
  date::detail::decimal_format_seconds<Duration> s_{tod.seconds() + tod.subseconds()};
  os << s_.seconds().count();
  if (s_.width > 0)
  {
    os << decimal_mark;
    save_ostream<CharT, Traits> _s(os);
    os.imbue(std::locale::classic());
    os.width(s_.width);
    os << s_.subseconds().count();
  }
  return os;
}

/*
 * Copying the `hh_mm_ss` `<<` operator to be able to use a user-supplied
 * `decimal_mark`.
 */
template <class CharT, class Traits, class Duration>
static
inline
std::basic_ostream<CharT, Traits>&
stream_tod(std::basic_ostream<CharT, Traits>& os,
           const date::hh_mm_ss<Duration>& tod,
           const CharT* decimal_mark)
{
  if (tod.is_negative())
    os << '-';
  if (tod.hours() < std::chrono::hours{10})
    os << '0';
  os << tod.hours().count() << ':';
  if (tod.minutes() < std::chrono::minutes{10})
    os << '0';
  os << tod.minutes().count() << ':';
  return stream_seconds(os, tod, decimal_mark);
}

// -----------------------------------------------------------------------------

template <class CharT, class Traits, class Duration>
std::basic_ostream<CharT, Traits>&
clock_to_stream(std::basic_ostream<CharT, Traits>& os,
                const CharT* fmt,
                const date::fields<Duration>& fds,
                const std::string* abbrev,
                const std::chrono::seconds* offset_sec,
                const std::pair<const std::string*, const std::string*>& month_names_pair,
                const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                const CharT* decimal_mark)
{
    using date::detail::save_ostream;
    using date::detail::get_units;
    using date::detail::extract_weekday;
    using date::detail::extract_month;
    using std::ios;
    using std::chrono::duration_cast;
    using std::chrono::seconds;
    using std::chrono::minutes;
    using std::chrono::hours;
    date::detail::save_ostream<CharT, Traits> ss(os);
    os.fill(' ');
    os.flags(std::ios::skipws | std::ios::dec);
    os.width(0);
    tm tm{};
    bool insert_negative = fds.has_tod && fds.tod.to_duration() < Duration::zero();
    const CharT* command = nullptr;
    CharT modified = CharT{};
    for (; *fmt; ++fmt)
    {
        switch (*fmt)
        {
        case 'a':
        case 'A':
            if (command)
            {
                if (modified == CharT{})
                {
                    tm.tm_wday = static_cast<int>(extract_weekday(os, fds));
                    if (os.fail())
                        return os;
                    os << weekday_names_pair.first[tm.tm_wday+7*(*fmt == 'a')];
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'b':
        case 'B':
        case 'h':
            if (command)
            {
                if (modified == CharT{})
                {
                    tm.tm_mon = static_cast<int>(extract_month(os, fds)) - 1;
                    os << month_names_pair.first[tm.tm_mon+12*(*fmt != 'B')];
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'c':
        case 'x':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.ok())
                        os.setstate(std::ios::failbit);
                    if (*fmt == 'c' && !fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (*fmt == 'c')
                    {
                        auto wd = static_cast<int>(extract_weekday(os, fds));
                        os << weekday_names_pair.first[static_cast<unsigned>(wd)+7]
                           << ' ';
                        os << month_names_pair.first[extract_month(os, fds)-1+12] << ' ';
                        auto d = static_cast<int>(static_cast<unsigned>(fds.ymd.day()));
                        if (d < 10)
                            os << ' ';
                        os << d << ' '
                           << date::make_time(duration_cast<seconds>(fds.tod.to_duration()))
                           << ' ' << fds.ymd.year();

                    }
                    else  // *fmt == 'x'
                    {
                        auto const& ymd = fds.ymd;
                        save_ostream<CharT, Traits> _(os);
                        os.fill('0');
                        os.flags(std::ios::dec | std::ios::right);
                        os.width(2);
                        os << static_cast<unsigned>(ymd.month()) << CharT{'/'};
                        os.width(2);
                        os << static_cast<unsigned>(ymd.day()) << CharT{'/'};
                        os.width(2);
                        os << static_cast<int>(ymd.year()) % 100;
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'C':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.year().ok())
                        os.setstate(std::ios::failbit);
                    auto y = static_cast<int>(fds.ymd.year());
                    save_ostream<CharT, Traits> _(os);
                    os.fill('0');
                    os.flags(std::ios::dec | std::ios::right);
                    if (y >= 0)
                    {
                      os.width(2);
                      os << y/100;
                    }
                    else
                    {
                      os << CharT{'-'};
                      os.width(2);
                      os << -(y-99)/100;
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'd':
        case 'e':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.day().ok())
                      os.setstate(std::ios::failbit);
                    auto d = static_cast<int>(static_cast<unsigned>(fds.ymd.day()));
                    save_ostream<CharT, Traits> _(os);
                    if (*fmt == CharT{'d'})
                      os.fill('0');
                    else
                      os.fill(' ');
                    os.flags(std::ios::dec | std::ios::right);
                    os.width(2);
                    os << d;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'D':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto const& ymd = fds.ymd;
                    save_ostream<CharT, Traits> _(os);
                    os.fill('0');
                    os.flags(std::ios::dec | std::ios::right);
                    os.width(2);
                    os << static_cast<unsigned>(ymd.month()) << CharT{'/'};
                    os.width(2);
                    os << static_cast<unsigned>(ymd.day()) << CharT{'/'};
                    os.width(2);
                    os << static_cast<int>(ymd.year()) % 100;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'F':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto const& ymd = fds.ymd;
                    save_ostream<CharT, Traits> _(os);
                    os.imbue(std::locale::classic());
                    os.fill('0');
                    os.flags(std::ios::dec | std::ios::right);
                    os.width(4);
                    os << static_cast<int>(ymd.year()) << CharT{'-'};
                    os.width(2);
                    os << static_cast<unsigned>(ymd.month()) << CharT{'-'};
                    os.width(2);
                    os << static_cast<unsigned>(ymd.day());
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'g':
        case 'G':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto ld = date::local_days(fds.ymd);
                    auto y = date::year_month_day{ld + date::days{3}}.year();
                    auto start = date::local_days((y-date::years{1})/date::December/date::Thursday[date::last]) +
                                 (date::Monday-date::Thursday);
                    if (ld < start)
                        --y;
                    if (*fmt == CharT{'G'})
                        os << y;
                    else
                    {
                        save_ostream<CharT, Traits> _(os);
                        os.fill('0');
                        os.flags(std::ios::dec | std::ios::right);
                        os.width(2);
                        os << std::abs(static_cast<int>(y)) % 100;
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'H':
        case 'I':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (insert_negative)
                    {
                        os << '-';
                        insert_negative = false;
                    }
                    auto hms = fds.tod;
                    auto h = *fmt == CharT{'I'} ? date::make12(hms.hours()) : hms.hours();
                    if (h < hours{10})
                      os << CharT{'0'};
                    os << h.count();
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'j':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (fds.ymd.ok() || fds.has_tod)
                    {
                        date::days doy;
                        if (fds.ymd.ok())
                        {
                            auto ld = date::local_days(fds.ymd);
                            auto y = fds.ymd.year();
                            doy = ld - date::local_days(y/date::January/1) + date::days{1};
                        }
                        else
                        {
                            doy = duration_cast<date::days>(fds.tod.to_duration());
                        }
                        save_ostream<CharT, Traits> _(os);
                        os.fill('0');
                        os.flags(std::ios::dec | std::ios::right);
                        os.width(3);
                        os << doy.count();
                    }
                    else
                    {
                        os.setstate(std::ios::failbit);
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'm':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.month().ok())
                        os.setstate(std::ios::failbit);
                    auto m = static_cast<unsigned>(fds.ymd.month());
                    if (m < 10)
                      os << CharT{'0'};
                    os << m;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'M':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (insert_negative)
                    {
                        os << '-';
                        insert_negative = false;
                    }
                    if (fds.tod.minutes() < minutes{10})
                      os << CharT{'0'};
                    os << fds.tod.minutes().count();
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'n':
            if (command)
            {
                if (modified == CharT{})
                    os << CharT{'\n'};
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'p':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (date::is_am(fds.tod.hours()))
                      os << ampm_names_pair.first[0];
                    else
                      os << ampm_names_pair.first[1];
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                }
                modified = CharT{};
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'Q':
        case 'q':
            // TODO: These seem to be duration format values. Replace with quarter formatters?
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    auto d = fds.tod.to_duration();
                    if (*fmt == 'q')
                        os << get_units<CharT>(typename decltype(d)::period::type{});
                    else
                        os << d.count();
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                }
                modified = CharT{};
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'r':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    date::hh_mm_ss<seconds> tod(duration_cast<seconds>(fds.tod.to_duration()));
                    save_ostream<CharT, Traits> _(os);
                    os.fill('0');
                    os.width(2);
                    os << date::make12(tod.hours()).count() << CharT{':'};
                    os.width(2);
                    os << tod.minutes().count() << CharT{':'};
                    os.width(2);
                    os << tod.seconds().count() << CharT{' '};
                    if (date::is_am(tod.hours()))
                      os << ampm_names_pair.first[0];
                    else
                      os << ampm_names_pair.first[1];
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                }
                modified = CharT{};
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'R':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (fds.tod.hours() < hours{10})
                        os << CharT{'0'};
                    os << fds.tod.hours().count() << CharT{':'};
                    if (fds.tod.minutes() < minutes{10})
                        os << CharT{'0'};
                    os << fds.tod.minutes().count();
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'S':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    if (insert_negative)
                    {
                        os << '-';
                        insert_negative = false;
                    }
                    stream_seconds(os, fds.tod, decimal_mark);
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 't':
            if (command)
            {
                if (modified == CharT{})
                    os << CharT{'\t'};
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'T':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    stream_tod(os, fds.tod, decimal_mark);
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'u':
            if (command)
            {
                if (modified == CharT{})
                {
                    auto wd = extract_weekday(os, fds);
                    os << (wd != 0 ? wd : 7u);
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'U':
            if (command)
            {
                if (modified == CharT{})
                {
                    auto const& ymd = fds.ymd;
                    if (!ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto ld = date::local_days(ymd);
                    auto st = date::local_days(date::Sunday[1]/date::January/ymd.year());
                    if (ld < st)
                      os << CharT{'0'} << CharT{'0'};
                    else
                    {
                      auto wn = duration_cast<date::weeks>(ld - st).count() + 1;
                      if (wn < 10)
                        os << CharT{'0'};
                      os << wn;
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'V':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto ld = date::local_days(fds.ymd);
                    auto y = date::year_month_day{ld + date::days{3}}.year();
                    auto st = date::local_days((y-date::years{1})/12/date::Thursday[date::last]) +
                      (date::Monday-date::Thursday);
                    if (ld < st)
                    {
                      --y;
                      st = date::local_days((y - date::years{1})/12/date::Thursday[date::last]) +
                      (date::Monday-date::Thursday);
                    }
                    auto wn = duration_cast<date::weeks>(ld - st).count() + 1;
                    if (wn < 10)
                      os << CharT{'0'};
                    os << wn;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'w':
            if (command)
            {
                auto wd = extract_weekday(os, fds);
                if (os.fail())
                    return os;
                if (modified == CharT{})
                {
                    os << wd;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'W':
            if (command)
            {
                if (modified == CharT{})
                {
                    auto const& ymd = fds.ymd;
                    if (!ymd.ok())
                        os.setstate(std::ios::failbit);
                    auto ld = date::local_days(ymd);
                    auto st = date::local_days(date::Monday[1]/date::January/ymd.year());
                    if (ld < st)
                      os << CharT{'0'} << CharT{'0'};
                    else
                    {
                      auto wn = duration_cast<date::weeks>(ld - st).count() + 1;
                      if (wn < 10)
                        os << CharT{'0'};
                      os << wn;
                    }
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'X':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.has_tod)
                        os.setstate(std::ios::failbit);
                    stream_tod(os, fds.tod, decimal_mark);
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'y':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.year().ok())
                      os.setstate(std::ios::failbit);
                    auto y = static_cast<int>(fds.ymd.year());
                    y = std::abs(y) % 100;
                    if (y < 10)
                        os << CharT{'0'};
                    os << y;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'Y':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (!fds.ymd.year().ok())
                        os.setstate(std::ios::failbit);
                    auto y = fds.ymd.year();
                    save_ostream<CharT, Traits> _(os);
                    os.imbue(std::locale::classic());
                    os << y;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'z':
            if (command)
            {
                if (offset_sec == nullptr)
                {
                    // Can not format %z with unknown offset
                    os.setstate(ios::failbit);
                    return os;
                }
                auto m = duration_cast<minutes>(*offset_sec);
                auto neg = m < minutes{0};
                m = date::abs(m);
                auto h = duration_cast<hours>(m);
                m -= h;
                if (neg)
                    os << CharT{'-'};
                else
                    os << CharT{'+'};
                if (h < hours{10})
                    os << CharT{'0'};
                os << h.count();
                if (modified != CharT{})
                    os << CharT{':'};
                if (m < minutes{10})
                    os << CharT{'0'};
                os << m.count();
                command = nullptr;
                modified = CharT{};
            }
            else
                os << *fmt;
            break;
        case 'Z':
            if (command)
            {
                if (modified == CharT{})
                {
                    if (abbrev == nullptr)
                    {
                        // Can not format %Z with unknown time_zone
                        os.setstate(ios::failbit);
                        return os;
                    }
                    for (auto c : *abbrev)
                        os << CharT(c);
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    modified = CharT{};
                }
                command = nullptr;
            }
            else
                os << *fmt;
            break;
        case 'E':
        case 'O':
            if (command)
            {
                if (modified == CharT{})
                {
                    modified = *fmt;
                }
                else
                {
                    os << CharT{'%'} << modified << *fmt;
                    command = nullptr;
                    modified = CharT{};
                }
            }
            else
                os << *fmt;
            break;
        case '%':
            if (command)
            {
                if (modified == CharT{})
                {
                    os << CharT{'%'};
                    command = nullptr;
                }
                else
                {
                    os << CharT{'%'} << modified << CharT{'%'};
                    command = nullptr;
                    modified = CharT{};
                }
            }
            else
                command = fmt;
            break;
        default:
            if (command)
            {
                os << CharT{'%'};
                command = nullptr;
            }
            if (modified != CharT{})
            {
                os << modified;
                modified = CharT{};
            }
            os << *fmt;
            break;
        }
    }
    if (command)
        os << CharT{'%'};
    if (modified != CharT{})
        os << modified;
    return os;
}

// -----------------------------------------------------------------------------

/*
 * Adding another template variation of `to_stream()` that parses from a ymd
 * and tod directly. This way we keep the precision when parsing large dates
 * (i.e. with year past 1970 +/- 292) that have nanoseconds
 */

template <class CharT, class Traits, class Duration>
inline
std::basic_ostream<CharT, Traits>&
clock_to_stream(std::basic_ostream<CharT, Traits>& os,
                const CharT* fmt,
                const date::year_month_day& ymd,
                const date::hh_mm_ss<Duration>& hms,
                const std::string* abbrev,
                const std::chrono::seconds* offset_sec,
                const std::pair<const std::string*, const std::string*>& month_names_pair,
                const std::pair<const std::string*, const std::string*>& weekday_names_pair,
                const std::pair<const std::string*, const std::string*>& ampm_names_pair,
                const CharT* decimal_mark)
{
  date::fields<Duration> fds{ymd, hms};
  return clock_to_stream(os, fmt, fds, abbrev, offset_sec, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark);
}

// -----------------------------------------------------------------------------

[[cpp11::register]]
cpp11::writable::strings format_time_point_cpp(const clock_field& calendar,
                                               const clock_field& seconds_of_day,
                                               const clock_field& nanoseconds_of_second,
                                               const cpp11::strings& zone,
                                               const cpp11::strings& format,
                                               const cpp11::strings& precision,
                                               const bool& naive,
                                               const bool& abbreviate_zone,
                                               const cpp11::strings& mon,
                                               const cpp11::strings& mon_ab,
                                               const cpp11::strings& day,
                                               const cpp11::strings& day_ab,
                                               const cpp11::strings& am_pm,
                                               const cpp11::strings& decimal_mark) {
  r_ssize size = calendar.size();

  cpp11::writable::strings out(size);

  cpp11::writable::strings zone_standard = zone_standardize(zone);
  std::string zone_name = cpp11::r_string(zone_standard[0]);
  const date::time_zone* p_time_zone = zone_name_load(zone_name);

  if (format.size() != 1) {
    clock_abort("`format` must have size 1.");
  }

  std::string string_format(format[0]);
  const char* c_format = string_format.c_str();

  enum precision precision_val = parse_precision(precision);

  std::basic_ostringstream<char> stream;
  stream.imbue(std::locale::classic());

  std::string zone_name_print;
  std::string* p_zone_name_print = nullptr;

  if (!naive) {
    // If zoned, default printable zone name to full provided zone name
    zone_name_print = (zone_name.size() == 0) ? zone_name_current() : zone_name;
    p_zone_name_print = &zone_name_print;
  }

  std::string month_names[24];
  const std::pair<const std::string*, const std::string*>& month_names_pair = fill_month_names(
    mon,
    mon_ab,
    month_names
  );

  std::string weekday_names[14];
  const std::pair<const std::string*, const std::string*>& weekday_names_pair = fill_weekday_names(
    day,
    day_ab,
    weekday_names
  );

  std::string ampm_names[2];
  const std::pair<const std::string*, const std::string*>& ampm_names_pair = fill_ampm_names(
    am_pm,
    ampm_names
  );

  std::string decimal_mark_string = decimal_mark[0];
  const char* decimal_mark_char = decimal_mark_string.c_str();

  // Default to no offset, which might change if formatting a zoned datetime
  std::chrono::seconds offset;
  std::chrono::seconds* p_offset = nullptr;

  for (r_ssize i = 0; i < size; ++i) {
    int elt_calendar = calendar[i];
    int elt_seconds_of_day = seconds_of_day[i];

    if (elt_calendar == r_int_na) {
      out[i] = r_chr_na;
      continue;
    }

    date::local_days elt_lday;
    std::chrono::seconds elt_ltod;

    if (naive) {
      elt_lday = date::local_days{date::days{elt_calendar}};
      elt_ltod = std::chrono::seconds{elt_seconds_of_day};
    } else {
      date::sys_days elt_sday{date::days{elt_calendar}};
      std::chrono::seconds elt_stod{elt_seconds_of_day};

      date::sys_seconds elt_ssec_floor{elt_sday};
      date::sys_seconds elt_ssec{elt_ssec_floor + elt_stod};

      date::sys_info info = p_time_zone->get_info(elt_ssec);

      offset = info.offset;
      p_offset = &offset;

      if (abbreviate_zone) {
        zone_name_print = info.abbrev;
        p_zone_name_print = &zone_name_print;
      }

      date::local_seconds elt_lsec{(elt_ssec + info.offset).time_since_epoch()};

      elt_lday = date::floor<date::days>(elt_lsec);
      date::local_seconds elt_lsec_floor{elt_lday};

      elt_ltod = elt_lsec - elt_lsec_floor;
    }

    // Reset stream
    stream.str(std::string());
    // Reset flags
    stream.clear();

    date::year_month_day elt_ymd{elt_lday};

    switch (precision_val) {
    case precision::second: {
      date::hh_mm_ss<std::chrono::seconds> elt_hms{elt_ltod};
      clock_to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark_char);
      break;
    }
    case precision::millisecond: {
      int elt_nanoseconds_of_second = nanoseconds_of_second[i];
      std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};
      std::chrono::milliseconds elt_millis = std::chrono::duration_cast<std::chrono::milliseconds>(elt_nanos);
      date::hh_mm_ss<std::chrono::milliseconds> elt_hms{elt_ltod + elt_millis};
      clock_to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark_char);
      break;
    }
    case precision::microsecond: {
      int elt_nanoseconds_of_second = nanoseconds_of_second[i];
      std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};
      std::chrono::microseconds elt_micros = std::chrono::duration_cast<std::chrono::microseconds>(elt_nanos);
      date::hh_mm_ss<std::chrono::microseconds> elt_hms{elt_ltod + elt_micros};
      clock_to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark_char);
      break;
    }
    case precision::nanosecond: {
      int elt_nanoseconds_of_second = nanoseconds_of_second[i];
      std::chrono::nanoseconds elt_nanos{elt_nanoseconds_of_second};
      date::hh_mm_ss<std::chrono::nanoseconds> elt_hms{elt_ltod + elt_nanos};
      clock_to_stream(stream, c_format, elt_ymd, elt_hms, p_zone_name_print, p_offset, month_names_pair, weekday_names_pair, ampm_names_pair, decimal_mark_char);
      break;
    }
    }

    if (stream.fail()) {
      out[i] = r_chr_na;
      continue;
    }

    out[i] = stream.str();
  }

  return out;
}
