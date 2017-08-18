#include <fcntl.h>
#include <unistd.h>

#include <linux/psp-sev.h>
#include<sys/ioctl.h>

#include <iostream>
using namespace std;

static int sev_fd = 0;

#define SEV_CERT_LEN 16384

int sev_platform_init()
{
	int ret;
	struct sev_issue_cmd sev_cmd;
	struct sev_user_data_init init_data;
	
	init_data.flags = 0;
	init_data.tmr_address = 0;
	init_data.tmr_length = 0;

	sev_cmd.cmd = SEV_USER_CMD_INIT;
	sev_cmd.data = (unsigned long)&init_data;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	return ret;
}

int sev_pek_gen()
{
	int ret;
	struct sev_issue_cmd sev_cmd;

	sev_cmd.cmd = SEV_USER_CMD_PEK_GEN;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	return ret;
}

int sev_pek_csr()
{
	int ret;
	struct sev_issue_cmd sev_cmd;
	struct sev_user_data_pek_csr pek_csr_data;
	char* csr_data = NULL;

	csr_data = new char[SEV_CERT_LEN];
	pek_csr_data.address = (unsigned long)csr_data;
	pek_csr_data.length = SEV_CERT_LEN;

	sev_cmd.cmd = SEV_USER_CMD_PEK_CSR;
	sev_cmd.data = (unsigned long)&pek_csr_data;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	delete csr_data;

	return ret;
}

int sev_pek_cert_import()
{
	int ret;
	struct sev_issue_cmd sev_cmd;
	struct sev_user_data_pek_cert_import pek_cert_data;
	char *pek_cert = NULL; 
	char *oca_cert = NULL;

	pek_cert = new char[SEV_CERT_LEN];
	oca_cert = new char[SEV_CERT_LEN];
	pek_cert_data.pek_cert_address = (unsigned long)pek_cert;
	pek_cert_data.pek_cert_length = SEV_CERT_LEN;
	pek_cert_data.oca_cert_address = (unsigned long)oca_cert;
	pek_cert_data.oca_cert_length = SEV_CERT_LEN;

	sev_cmd.cmd = SEV_USER_CMD_PEK_CERT_IMPORT;
	sev_cmd.data = (unsigned long)&pek_cert_data;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	delete pek_cert;
	delete oca_cert;

	return ret;
}

int sev_pdh_gen()
{
	int ret;
	struct sev_issue_cmd sev_cmd;

	sev_cmd.cmd = SEV_USER_CMD_PDH_GEN;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	return ret;
}

int sev_pdh_cert_export()
{
	int ret;
	struct sev_issue_cmd sev_cmd;
	struct sev_user_data_pdh_cert_export pdh_cert_data;
	char *pdh_cert = NULL; 
	char *cert_chain= NULL;

	pdh_cert = new char[SEV_CERT_LEN];
	cert_chain = new char[SEV_CERT_LEN*3];
	pdh_cert_data.pdh_cert_address = (unsigned long)pdh_cert;
	pdh_cert_data.pdh_cert_length = SEV_CERT_LEN;
	pdh_cert_data.cert_chain_address = (unsigned long)cert_chain;
	pdh_cert_data.cert_chain_length = SEV_CERT_LEN;

	sev_cmd.cmd = SEV_USER_CMD_PDH_CERT_EXPORT;
	sev_cmd.data = (unsigned long)&pdh_cert_data;

	ret = ioctl(sev_fd, SEV_ISSUE_CMD, &sev_cmd);
	cout << __FUNCTION__ << ": ret=" << ret << "\n";

	delete pdh_cert;
	delete cert_chain;

	return ret;
}

int main()
{
	sev_fd = open("/dev/sev1", O_RDWR);
	if(sev_fd < 0)
	{
		cout << "failed to open /dev/sev1!\n";
		return -1;
	}

	sev_platform_init();
	sev_pek_gen();
	sev_pek_csr();
	sev_pek_cert_import();
	sev_pdh_gen();
	sev_pdh_cert_export();
	
	return 0;
}
