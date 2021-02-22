#pragma once

//! Функция комбинирования нескольких хэш значений
/*! Данная функция предназначена для комбинирования нескольких хэш значений\n
    Пример:\n
      \code
            struct Person
            {
                std::string m_name, m_surname;
                uint8_t m_age;
            };


            template <> struct
            hash<Person> {
                size_t operator()(const Person &obj) const noexcept
                {
                    return hash_combine(obj.m_name, obj.m_surname, obj.m_age);
                }
            };

            void foo()
            {
                Person obj { "Name", "Surname", 24 };
                const auto hash_value = std::hash<decltype(obj)>{}(obj);
            }
      \endcode

*/

template <typename T, typename ...Rest>
size_t hash_combine(const T &obj1, const Rest &...objN)
{
    size_t res = 0;
    res ^= std::hash<T>{}(obj1) + 0x9e3779b9 + (res << 6) + (res >> 2);
    ( ... , (res ^= std::hash<Rest>{}(objN) + 0x9e3779b9 + (res << 6) + (res >> 2)) );
    return res;
}
