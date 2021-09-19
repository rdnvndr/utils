#pragma once

#include "bimapiterator.h"
#include "bimappair.h"

#include <set>
#include <stddef.h>
#include <stdexcept>

//! Двунаправленный ассоциативный контейнер, построенный на базе std::set
template<typename Key, typename T>
class BiMap
{
public:
    //! Тип ключа
    using key_type               = Key;

    //! Тип значения
    using value_type             = std::pair<Key, T&>;

    //! Ссылка на значение
    using reference	             = value_type&;

    //! Константная ссылка на значение
    using const_reference        = const value_type&;

    //! Итератор
    using iterator               = BiMapIterator<Key, T&>;

    //! Константный итератор
    using const_iterator         = BiMapIterator<const Key, const T&>;

    //! Обратный итератор
    using reverse_iterator       = std::reverse_iterator<iterator>;

    //! Обратный константный итератор
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //! Конструктор по умолчанию
    BiMap() {}


    //! Возвращает итератор на начало контейнера
    iterator begin();

    //! Возвращает итератор на конец контейнера
    iterator end();

    //! Возвращает константный итератор на начало контейнера
    const_iterator cbegin() const;

    //! Возвращает константный итератор на конец контейнера
    const_iterator cend() const;

    //! Возвращает обратный итератор на начало контейнера
    reverse_iterator rbegin();

    //! Возвращает обратный итератор на конец контейнера
    reverse_iterator rend();

    //! Возвращает константный обратный итератор на начало контейнера
    const_reverse_iterator crbegin();

    //! Возвращает константный обратный итератор на конец контейнера
    const_reverse_iterator crend();


    //! Производит поиск по ключу и возвращает итератор на результат
    iterator find_key(const Key& key) const;

    //! Производит поиск по значению и возвращает итератор на результат
    iterator find_value(const T& value) const;

    //! Вставляет ключ и значение в контейнер и возвращает итератор на вставленную пару
    iterator insert(const Key& key, const T& value);

    //! Вставляет пару ключ-значение в контейнер и возвращает итератор на вставленную пару
    iterator insert(const value_type& value);

    //! Удаляет элемент по ключу и возвращает итератор на элемент после удалённого
    iterator erase_key(const key_type& key);

    //! Удаляет элемент по значению и возвращает итератор на элемент после удалённого
    iterator erase_value(const value_type& value);

    //! Удаляет элемент по константному итератору и возвращает итератор на элемент после удалённого
    iterator erase(const_iterator pos);

    //! Удаляет элемент по итератору и возвращает итератор на элемент после удалённого
    iterator erase(iterator pos);

    //! Проверяет существует ли элемент с указанным ключом
    bool has_key(const Key& key) const;

    //! Проверяет существует ли элемент с указанным значением
    bool has_value(const T& value) const;

    //! Возвращает значение по ключу
    T value(const Key& key) const;

    //! Возвращает ключ по значению
    Key key(const T& value) const;

    //! Возвращает размер контейнера
    size_t size() const;

    //! Очищает контейнер
    void clear();

private:
    //! Множество ключей со ссылкой на значение
    std::set<BiMapPair<Key, T&>> _key;

    //! Множество значений со ссылкой на ключ
    std::set<BiMapPair<T, const Key*>> _value;
};

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::begin()
{
    return iterator(_key.begin());
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::end()
{
    return iterator(_key.end());
}

template<typename Key, typename T>
typename BiMap<Key, T>::const_iterator BiMap<Key, T>::cbegin() const
{
    return const_iterator(_key.begin());
}

template<typename Key, typename T>
typename BiMap<Key, T>::const_iterator BiMap<Key, T>::cend() const
{
    return const_iterator(_key.end());
}

template<typename Key, typename T>
typename BiMap<Key, T>::reverse_iterator BiMap<Key, T>::rbegin()
{
    return BiMap<Key, T>::reverse_iterator(this->end());
}

template<typename Key, typename T>
typename BiMap<Key, T>::reverse_iterator BiMap<Key, T>::rend()
{
    return BiMap<Key, T>::reverse_iterator(this->begin());
}

template<typename Key, typename T>
typename BiMap<Key, T>::const_reverse_iterator BiMap<Key, T>::crbegin()
{
    return BiMap<Key, T>::const_reverse_iterator(this->cend());
}

template<typename Key, typename T>
typename BiMap<Key, T>::const_reverse_iterator BiMap<Key, T>::crend()
{
    return BiMap<Key, T>::const_reverse_iterator(this->cbegin());
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::find_key(const Key &key) const
{
    auto key_it = _key.find(key);
    return iterator(key_it);
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::find_value(const T &value) const
{
    auto value_it = _value.find(value);
    auto key_it   = (value_it != _value.end()) ? _key.find(*value_it->second) : _key.end();
    return iterator(key_it);
}

template<class Key, class T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::insert(const Key &key, const T &value)
{
    auto value_it = _value.emplace(key, nullptr).first;
    auto key_it   = _key.emplace(key, *const_cast<T *>(&value_it->first)).first;
    auto node = _value.extract(value_it);
    node.value().second = &key_it->first;
    _value.insert(std::move(node));

    return key_it;
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::insert(const value_type &value)
{
    return insert(value.first, value.second);
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::erase_key(const key_type &key)
{
    auto key_it = _key.find(key);
    if (key_it != _key.end())
    {
        auto value_it = _value.find(*key_it->second);
        _value.erase(value_it);
        return iterator(_key.erase(key_it));
    } else {
        return iterator(_key.end());
    }
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::erase_value(const value_type &value)
{
    auto value_it = _value.find(value);
    if (value_it != _value.end())
    {
        auto key_it = _key.find(*value_it->second);
        _value.erase(value_it);
        return iterator(_key.erase(key_it));
    } else {
        return iterator(_key.end());
    }
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::erase(const_iterator pos)
{
    return iterator(_key.erase(pos));
}

template<typename Key, typename T>
typename BiMap<Key, T>::iterator BiMap<Key, T>::erase(iterator pos)
{
    return iterator(_key.erase(pos));
}

template<typename Key, typename T>
bool BiMap<Key, T>::has_key(const Key &key) const
{
    return _key.find(key) != _key.end();
}

template<typename Key, typename T>
bool BiMap<Key, T>::has_value(const T &value) const
{
    return _value.find(value) != _value.end();
}

template<typename Key, typename T>
T BiMap<Key, T>::value(const Key &key) const
{
    T value {};
    auto key_it = _key.find({key, value});
    if (key_it != _key.end())
        return key_it->second;
    else
        throw std::out_of_range("BiMap::value");
}

template<typename Key, typename T>
Key BiMap<Key, T>::key(const T &value) const
{
    auto value_it = _value.find({value, nullptr});
    if (value_it != _value.end())
        return *value_it->second;
    else
        throw std::out_of_range("BiMap::key");
}

template<typename Key, typename T>
size_t BiMap<Key, T>::size() const
{
    return _key.size();
}

template<typename Key, typename T>
void BiMap<Key, T>::clear()
{
    _key.clear();
    _value.clear();
}
