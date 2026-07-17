# Basic Node usage
This example shows basic usage of Node  

```cpp title="File examples/basic_node_usage.cpp"
--8<-- "examples/basic_node_usage.cpp"
```

1. Node does not load the config on its own (from a file or `:::cpp std::stdin`) - this is done for versatility
2. Node loads `:::cpp io_context` with work completely, so `:::cpp io_context.run()` never stops. Need an external way to call `:::cpp io_context.stop`
3. sleep_for is needed to avoid busy‑wait
4. Node mutates the config, which is why it needs to be saved

!!! info "Explanation"
    First you need to load the config and create `:::cpp boost::asio::io_context` or any other executor compatible with `:::cpp boost::asio::any_io_executor`.
    Next, you create the Node itself and can use it

*[Node]: Example theme
