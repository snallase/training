#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/platform_device.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/ethtool.h>
#include <linux/delay.h>
#include <linux/rtnetlink.h>
#include <linux/mii.h>
#include <linux/crc32.h>
#include <linux/bitops.h>
#include <linux/prefetch.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>

#define DRVNAME "ethd"
#define TX_TIMEOUT  (2*HZ)

struct eth_private {
	struct net_device *dev;
	struct napi_struct napi;
	struct net_device_stats stats;
	struct platform_device *pdev;

	spinlock_t lock;
	int device;
};

struct platform_device *platform_dev;

static int netdev_open(struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
	return 0;
}

static void ns_tx_timeout(struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
}

static netdev_tx_t start_tx(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
	return 0;
}

static void netdev_rx(struct net_device *dev, int *work_done, int work_to_do)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
}

static void netdev_tx_done(struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
}

static int change_mtu(struct net_device *dev, int new_mtu)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
	return 0;
}

static void set_rx_mode(struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
}

static struct net_device_stats *get_stats(struct net_device *dev)
{
	struct eth_private *ep = netdev_priv(dev);

	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);

	spin_lock_irq(&ep->lock);

	spin_unlock_irq(&ep->lock);

	return &ep->stats;
}

static int netdev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
	return 0;
}

static int netdev_close(struct net_device *dev)
{
	printk(KERN_ALERT "%s:%s:Entry\n", DRVNAME, __FUNCTION__);
	return 0;
}

static const struct net_device_ops ethd_netdev_ops = {
	.ndo_open = netdev_open,
	.ndo_stop = netdev_close,
	.ndo_start_xmit = start_tx,
	.ndo_get_stats = get_stats,
#if 0
	.ndo_set_rx_mode = set_rx_mode,
	.ndo_change_mtu = change_mtu,
	.ndo_do_ioctl = netdev_ioctl,
	.ndo_tx_timeout = ns_tx_timeout,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_validate_addr = eth_validate_addr,
#endif
};

static int eth_dummy_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct eth_private *ep;
	int err;

	dev = alloc_etherdev(sizeof(struct eth_private));

	printk(KERN_ALERT "Platform eth dev pointer : %p\n", dev);

	if (dev == NULL) {
		printk(KERN_ALERT "ethd: etherdev allocation failed\n");
		return -1;
	}

	platform_set_drvdata(pdev, dev);
	dev->flags |= IFF_NOARP;
	dev->flags &= ~IFF_MULTICAST;

	ep = netdev_priv(dev);
	ep->pdev = pdev;

	dev->netdev_ops = &ethd_netdev_ops;
	dev->watchdog_timeo = TX_TIMEOUT;

	spin_lock_init(&ep->lock);

	err = register_netdev(dev);
	if (err) {
		free_netdev(dev);
		printk(KERN_ALERT "ethd: netdev register failed \n");
		return -ENODEV;
        }

	printk(KERN_ALERT "ethd: probe is successfully completed\n");

	return 0;
}

static int eth_dummy_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	unregister_netdev(dev);

	free_netdev(dev);

	return 0;
}

static struct platform_driver eth_dummy_driver = {
	.probe = eth_dummy_probe,
	.remove = eth_dummy_remove,
	.driver = {
		   .name = DRVNAME,
		   .owner = THIS_MODULE,
		   },
};

static int eth_dummy_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&eth_dummy_driver);

	if (ret < 0) {
		printk(KERN_ALERT
		       "Ethernet Dummy Driver init failed with :%d\n", ret);
		return -1;
	}

	platform_dev = platform_device_register_simple(DRVNAME, -1, NULL, 0);
	if (IS_ERR(platform_dev)) {
		ret = PTR_ERR(platform_dev);
		platform_driver_unregister(&eth_dummy_driver);
		printk(KERN_ALERT
		       "Ethernet Dummy Driver: device register simple failed with :%d\n", ret);
		goto out;
	}

	printk(KERN_ALERT "Ethernet Driver registered successfully\n");

 out:
	return ret;
}

static void eth_dummy_exit(void)
{
	platform_device_unregister(platform_dev);
	platform_driver_unregister(&eth_dummy_driver);

	printk(KERN_ALERT "Ethernet Driver unregistered successfully\n");
}

module_init(eth_dummy_init);
module_exit(eth_dummy_exit);

MODULE_AUTHOR("Singaravelan Nallasellan");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Ethernet MAC Dummy Driver");
