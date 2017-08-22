#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>
#include <linux/mii.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/ioctl.h>  
#include <net/if.h>  
#include <linux/sockios.h>  
#include <linux/types.h>  
#include <netinet/in.h>  
#include <unistd.h>  
  
static int sockfd;  

static void usage()
{
	printf("Usage:\n\
    mdio read [interface] [reg]\n\
    mdio write [interface] [reg] [val]\n");

	printf("\n");

	printf("Example:\n\
    mdio read eth0 0x01\n\
    mdio write eth0 0x01 0x3100\n");
}
  
int main(int argc, char *argv[])
{  
    struct mii_ioctl_data *mii_iodata = NULL;  
    struct ifreq ifr;  
    int ret;  

    if(argc < 4 || argc > 5 || !strcmp(argv[1], "-h")){  
        usage();  
		return -1;
    }  
      
    memset(&ifr, 0, sizeof(ifr));  
    strncpy(ifr.ifr_name, argv[2], IFNAMSIZ - 1);  
  
    sockfd = socket(PF_LOCAL, SOCK_DGRAM, 0);  
	if (sockfd < 0) {
		fprintf(stderr, "failed to create socker, err:%s(%d)\n", strerror(sockfd), sockfd);
		return -1;
	}
  
    /* get phy address */
    ret = ioctl(sockfd, SIOCGMIIPHY, &ifr);  
	if (ret < 0) {
		fprintf(stderr, "failed to get phy address, err:%s(%d)\n", strerror(errno), errno);
		goto exit;
	}
  
    mii_iodata = (struct mii_ioctl_data*)&ifr.ifr_data;  

    /* read phy register */
    if (!strcmp(argv[1], "read") && (argc == 4)) {  
        mii_iodata->reg_num = (uint16_t) strtoul(argv[2], NULL, 0);  
          
        ret = ioctl(sockfd, SIOCGMIIREG, &ifr);  
		if (ret < 0) {
			fprintf(stderr, "failed to read phy register, err:%s(%d)\n", strerror(ret), ret);
			goto exit;
		}
      
        printf("read phyaddr: 0x%02x, reg: 0x%02x,  value: 0x%x\n", 
				mii_iodata->phy_id, 
				mii_iodata->reg_num, 
				mii_iodata->val_out);  
    /* write value to phy register */
    } else if (!strcmp(argv[1], "write") && (argc == 5)) {  
        mii_iodata->reg_num = (uint16_t)strtoul(argv[3], NULL, 0);  
        mii_iodata->val_in = (uint16_t)strtoul(argv[4], NULL, 0);  

        ret = ioctl(sockfd, SIOCSMIIREG, &ifr);  
		if (ret < 0) {
			fprintf(stderr, "failed to write phy register, err:%s(%d)\n", strerror(ret), ret);
			goto exit;
		}
  
        printf("write phyaddr: 0x%02x, reg: 0x%02x, value: 0x%x\n", 
				mii_iodata->phy_id, 
				mii_iodata->reg_num, 
				mii_iodata->val_in);  
    }  
  
exit:  
    close(sockfd);  
    return 0;  
}  
