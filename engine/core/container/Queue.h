#pragma once

#include <queue>
#include <mutex>
#include <optional>

template<typename T>
class ThreadedQueue {
public:
    ThreadedQueue() = default;

    void push(const T& object) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(object);
    }

    void push(T&& object) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(std::move(object));
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.emplace(std::forward<Args>(args)...);
    }

    bool tryPop(T& out) {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_queue.empty())
            return false;

        out = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.size();
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);

        while (!m_queue.empty())
            m_queue.pop();
    }

private:
    mutable std::mutex m_mutex;
    std::queue<T> m_queue;
};
