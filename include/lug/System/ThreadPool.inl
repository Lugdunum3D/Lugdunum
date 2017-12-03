template<typename Func, typename ...Args>
auto ThreadPool::enqueue(Func&& func, Args&&... args) {
    // Bind function to arguments
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    using returnType = std::result_of_t<decltype(task)()>;

    // Create future task
    // Make it share_ptr so it's not detroyed when exiting ThreadPool::enqueue
    auto futureTask = std::make_shared<std::packaged_task<returnType()> >(std::move(task));

    // Add lambda which execute the task
    // futureTask will be destroyed when finished
    _queue.push([futureTask]{
        (*futureTask)();
    });

    // Notify one worker to execute the task
    _condition.notify_one();

    return futureTask->get_future();
}
