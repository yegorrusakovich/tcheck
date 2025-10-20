#include <gtest/gtest.h>

#include <mtch/matchers.hpp>

TEST(DateValidation, Base) {
  tcheck::DateMatcher matcher;
  ASSERT_TRUE(matcher.IsValid("14.09.2006"));
  ASSERT_TRUE(matcher.IsValid("14 сентября 2006"));
  ASSERT_TRUE(matcher.IsValid("с 2013 по 2020 годы"));
  ASSERT_TRUE(matcher.IsValid("с 2013 по 2020"));
  ASSERT_TRUE(matcher.IsValid("в 2017 году"));
}

TEST(DateSearch, Base) {
  using namespace std::string_view_literals;

  auto source =
      "Нобелевская премия в 2017 году была получена Ивановым И. И."
      "С августа 2019 года по декабрь 2021 года необходимо достигнуть апогея."
      "Суммарно с 2020 года по первое полугодие 2021 года было обнаружено..."
      "Этим утром мы отправились на рыбалку"sv;
  auto expected = std::set{
      "в 2017 году"sv,
      "с августа 2019 года по декабрь 2021 года"sv,
      "с 2020 года по первое полугодие 2021 года"sv,
      "Этим утром"sv,
  };
  tcheck::DateMatcher matcher;

  auto actual = matcher.Match(source);

  ASSERT_EQ(expected.size(), actual.size()) << "Wrong matched count";
  for (auto text_date : actual) {
    ASSERT_TRUE(expected.contains(text_date));
  }
}
