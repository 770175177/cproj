#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <linux/videodev2.h>

#define VIDEO_FILE           "/dev/video1"
#define PICTURE_FILE         "pic.jpg"
#define PICTURE_WIDTH        640
#define PICTURE_HEIGTH       480
#define REQ_KERNEL_BUFFERS   5

#if 1
#define PICTURE_FORMAT       V4L2_PIX_FMT_MJPEG
#else
#define PICTURE_FORMAT       V4L2_PIX_FMT_YUYV
#endif

typedef struct {
	void *start;
	int length;
}BUF_USER;

BUF_USER user_buf[REQ_KERNEL_BUFFERS];

int get_capability(int fd);
int set_capability(int fd);
int request_buffers(int fd);
int mmap_buffers(int fd);
int start_capture(int fd);
int read_pictures(int fd);
int stop_capture(int fd);

int main(int argc, const char *argv[])
{
	int fd, ret;

	/* 1.open file */
	printf("\n/************** 1.open file ****************/\n");
	fd = open(VIDEO_FILE, O_RDWR);
	if(fd < 0)
	{
		perror("open video file");
		return -1;
	}	
	printf("open video file %s ok\n", VIDEO_FILE);
	
	/* 2.capability */
	printf("\n/************** 2.capability ***************/\n");
	ret = get_capability(fd);
	if (ret < 0)
		return ret;
	printf("get video capability ok\n");
	
	/* 3.set video capture ability */
	printf("\n/*************** 3.setting ****************/\n");
	ret = set_capability(fd);
	if (ret < 0)
		return ret;
	printf("set video capability ok\n");

	printf("\n/*************** 4.apply buffer ***********/\n");
	ret = request_buffers(fd);
	if (ret < 0)
		return ret;
	printf("request kernel buffers num %d ok\n", REQ_KERNEL_BUFFERS);

	printf("\n/*************** 5.mmap mem/put to fifo ***/\n");
	ret = mmap_buffers(fd);
	if (ret < 0)
		return ret;
	printf("map buffers and enqueue them to fifo ok\n");
	
	printf("\n/*************** 6.start capture  *********/\n");
	ret = start_capture(fd);
	if (ret < 0)
		return ret;
	printf("start capture pictures ok\n");

	printf("\n/*************** 7.get pictures ***********/\n");
	ret = read_pictures(fd);
	if (ret < 0)
		return ret;
	printf("read pictures ok\n");

	printf("\n/*************** 8.stop capture ***********/\n");
	ret = stop_capture(fd);
	if (ret < 0)
		return ret;
	printf("stop capture pictures ok\n");
		
}

int get_capability(int fd)
{
	int ret = 0;
	struct v4l2_capability cap;

	memset(&cap, 0, sizeof(struct v4l2_capability));
	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if(ret < 0)
	{
		perror("query capability");
		return -1;
	}
	printf("driver       : %s\n", cap.driver);
	printf("card         : %s\n", cap.card);
	printf("bus_info     : %s\n", cap.bus_info);
	printf("version      : %d.%d.%d\n", cap.version >> 16, (cap.version >> 8) & 0xFF, cap.version & 0xFF );
	printf("capabilities : 0x%x\n", cap.capabilities);
	if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
	{
		printf("               capture\n");
	}
	if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
	{
		printf("               output\n");
	}
	if (cap.capabilities & V4L2_CAP_VIDEO_OVERLAY)
	{
		printf("               overlay\n");
	}
	if (cap.capabilities & V4L2_CAP_STREAMING)
	{
		printf("               streaming\n");
	}
	if (cap.capabilities & V4L2_CAP_READWRITE)
	{
		printf("               read_write\n");
	}

	return 0;
}

int set_capability(int fd)
{
	int ret = 0;
	struct v4l2_input input;
	struct v4l2_fmtdesc fmtdesc;
	struct v4l2_format v4l2_fmt;
	struct v4l2_frmsizeenum frmsize;
	struct v4l2_frmivalenum frmival;

	/* 1.1)enum support input device, and print input name */
	/* uvc video not need set input, fallowing is a tempelate */
	memset(&input, 0, sizeof(input));
	input.index = 0;
	while(!ioctl(fd, VIDIOC_ENUMINPUT, &input))
	{
		printf("------------- input %d ---------------\n", input.index);
		printf("name         : %s\n", input.name);
		printf("type         : 0x%x\n", input.type);
		printf("audioset     : 0x%x\n", input.audioset);
		printf("tuner        : 0x%x\n", input.tuner);
		//printf("std_id       : %lu\n", input.std);
		printf("status       : %d\n", input.status);
		input.index++;
	}

	/* 1.2)set input device */
	input.index = 0;
	ret = ioctl(fd, VIDIOC_S_INPUT, &input);
	if(ret < 0)
	{
		perror("set input capability");
		return ret;
	}

	/* 2.1)enum support MJPEG picture framesize */
	memset(&frmsize, 0, sizeof(struct v4l2_frmsizeenum));
	frmsize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	frmsize.index = 0;
	frmsize.pixel_format = V4L2_PIX_FMT_MJPEG;
	while(!ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize))
	{
		printf("------------- MJPEG pixel format %d -------------\n", frmsize.index);
		printf("width        : %d\n", frmsize.discrete.width);
		printf("height       : %d\n", frmsize.discrete.height);
		printf("frm_interval :\n");
		frmival.index = 0;
		frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		frmival.pixel_format = V4L2_PIX_FMT_MJPEG;
		frmival.width = frmsize.discrete.width;
		frmival.height = frmsize.discrete.height;
		while(!ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
		{
			printf("               %dfps\n", frmival.discrete.denominator / frmival.discrete.numerator);
			frmival.index++;
		}
		frmsize.index++;
	}

	/* 2.2)enum support YUV422 picture framesize */
	memset(&frmsize, 0, sizeof(struct v4l2_frmsizeenum));
	frmsize.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	frmsize.index = 0;
	frmsize.pixel_format = V4L2_PIX_FMT_YUYV;
	while(!ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize))
	{
		printf("------------- YUYV pixel format %d -------------\n", frmsize.index);
		printf("width        : %d\n", frmsize.discrete.width);
		printf("height       : %d\n", frmsize.discrete.height);
		printf("frm_interval :\n");
		frmival.index = 0;
		frmival.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		frmival.pixel_format = V4L2_PIX_FMT_YUYV;
		frmival.width = frmsize.discrete.width;
		frmival.height = frmsize.discrete.height;
		while(!ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival))
		{
			printf("               %dfps\n", frmival.discrete.denominator / frmival.discrete.numerator);
			frmival.index++;
		}
		frmsize.index++;
	}

	/* 3.1)enum support picture pixel format */
	memset(&fmtdesc, 0, sizeof(fmtdesc));
	fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmtdesc.index = 0;
	while(!ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
	{
		printf("------------- fmtdesc %d -------------\n", fmtdesc.index);
		printf("name         : %s\n", fmtdesc.description);
		printf("type         : 0x%x\n", fmtdesc.type);
		printf("flags        : 0x%x\n", fmtdesc.flags);
		printf("pixelformat  : 0x%x\n", fmtdesc.pixelformat);
		fmtdesc.index++;
	}

	/* 3.2)set picture pixel format */
	memset(&v4l2_fmt, 0, sizeof(v4l2_fmt));
	v4l2_fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2_fmt.fmt.pix.width = PICTURE_WIDTH;
	v4l2_fmt.fmt.pix.height = PICTURE_HEIGTH;
	v4l2_fmt.fmt.pix.pixelformat = PICTURE_FORMAT;
	v4l2_fmt.fmt.pix.field = V4L2_FIELD_ANY;
	ret = ioctl(fd, VIDIOC_S_FMT, &v4l2_fmt);
	if (ret < 0)
	{
		perror("set pixel format");
		return ret;
	}
	printf("------------- set pixel format -------------\n");
	printf("width        : %d\n", v4l2_fmt.fmt.pix.width);
	printf("height       : %d\n", v4l2_fmt.fmt.pix.height);

	/* 4.1)check picture pixel format */
	printf("------------- check pixel format -----------\n");
	ret = ioctl(fd, VIDIOC_G_FMT, &v4l2_fmt);
	if (ret < 0)
	{
		perror("get pixel format");
		return ret;
	}
	else if(v4l2_fmt.fmt.pix.width == PICTURE_WIDTH && v4l2_fmt.fmt.pix.height == PICTURE_HEIGTH)
	{
		printf("actual pixel format is\n");
		printf("width        : %d\n", v4l2_fmt.fmt.pix.width);
		printf("height       : %d\n", v4l2_fmt.fmt.pix.height);
	}
	else
	{
		printf("setting pixel format is not effect\n");
	}

	printf("-------------------------------------\n");
	
	return 0;
}

int request_buffers(int fd)
{
	int ret = 0;
	struct v4l2_requestbuffers req;

	/* streaming is that kernel space has a buffer fifo, then user space should map a buffer to kernel buffer */

	memset(&req, 0, sizeof(req));
	req.count = REQ_KERNEL_BUFFERS;
	req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	ret = ioctl(fd, VIDIOC_REQBUFS, &req);
	if (ret < 0)
	{
		perror("request buffers");
		return ret;
	}

	return 0;
}

int mmap_buffers(int fd)
{
	int i;
	int ret = 0;
	struct v4l2_buffer v4l2_buf;

	for (i = 0; i < REQ_KERNEL_BUFFERS; i++)
	{
		memset(&v4l2_buf, 0, sizeof(struct v4l2_buffer));
		v4l2_buf.index = i;
		v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2_buf.memory = V4L2_MEMORY_MMAP;
		/* check buffer info */
		ret = ioctl(fd, VIDIOC_QUERYBUF, &v4l2_buf);
		if (ret < 0)
		{
			perror("query buffer");
			return ret;
		}
		/* mmap */
		user_buf[i].length = v4l2_buf.length;
		user_buf[i].start = mmap(NULL, v4l2_buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, v4l2_buf.m.offset);
		if (user_buf[i].start == NULL)
		{
			perror("mmap addr");
			return -1;
		}

		/* put all buffers to fifo */
		ret = ioctl(fd, VIDIOC_QBUF, &v4l2_buf);
		if (ret < 0)
		{
			perror("enqueue buffers");
			return ret;
		}
	}

	return 0;
}

int start_capture(int fd)
{
	int ret = 0;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if (ret < 0)
	{
		perror("start stream");
		return ret;
	}

	return 0;
}

int stop_capture(int fd)
{
	int ret = 0;
	int i;
	enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

	ret = ioctl(fd, VIDIOC_STREAMOFF, &type);
	if (ret < 0)
	{
		perror("stop stream");
		return ret;
	}

	for (i = 0; i < REQ_KERNEL_BUFFERS; i++)
	{
		munmap(user_buf[i].start, user_buf[i].length);
	}

	close(fd);

	return 0;
}

int read_pictures(int fd)
{
	int ret = 0;
	int i = 1;
	FILE *fp;
	struct pollfd poll_fds[1];
	struct v4l2_buffer readbuf;

	poll_fds[0].fd = fd;
	poll_fds[0].events = POLLIN;   // wait for readable

	while(i)
	{
		poll(poll_fds, 1, -1);
		printf("------------------ get from fifo ------------\n");
		readbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		readbuf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(fd, VIDIOC_DQBUF, &readbuf);
		if (ret < 0)
		{
			perror("dequeue buffer");
			return ret;
		}

		//printf("------------------ read picture -------------\n");
		fp = fopen(PICTURE_FILE, "w+");
		if (fp == NULL)
		{
			perror("open picture file %s");
			return -1;
		}
		fwrite(user_buf[readbuf.index].start, readbuf.length, 1, fp);
		fclose(fp);
	
		//printf("------------------ put buf to fifo ----------\n");
		ret = ioctl(fd, VIDIOC_QBUF, &readbuf);
		if (ret < 0)
		{
			perror("enqueue buffer");
			return ret;
		}
		i--;
	}

	return 0;
}

