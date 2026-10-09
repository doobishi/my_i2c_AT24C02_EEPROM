#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/nvmem-consumer.h>
#include <linux/slab.h>
#include <linux/of.h>   

static int demo_probe(struct platform_device *pdev)
{
	struct nvmem_cell *cell;
	size_t len;
	u8 *buf;

	cell = devm_nvmem_cell_get(&pdev->dev, "serial");
	if (IS_ERR(cell))
		return dev_err_probe(&pdev->dev, PTR_ERR(cell),
				     "no serial cell\n");

	buf = nvmem_cell_read(cell, &len);
	if (IS_ERR(buf))
		return PTR_ERR(buf);

	dev_info(&pdev->dev, "serial (%zu bytes): %*phN\n", len, (int)len, buf);
	kfree(buf);
	return 0;
}

static const struct of_device_id demo_of_match[] = {
	{ .compatible = "fang,nvmem-demo" },
	{ }
};

MODULE_DEVICE_TABLE(of, demo_of_match);

static struct platform_driver demo_driver = {
	.probe = demo_probe,
	.driver = { .name = "nvmem-demo", .of_match_table = demo_of_match },
};

module_platform_driver(demo_driver);
MODULE_AUTHOR("doobi-shih");
MODULE_LICENSE("GPL");