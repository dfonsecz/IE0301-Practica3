#include <gst/gst.h>
#include <glib.h>

int main(int argc, char *argv[]) {
    GstElement *pipeline;
    GMainLoop *loop;

    gst_init(&argc, &argv);
    loop = g_main_loop_new(NULL, FALSE);

    const gchar *pipeline_description =
        "filesrc location=/opt/nvidia/deepstream/deepstream/samples/streams/sample_1080p_h264.mp4 ! \
         qtdemux ! h264parse ! nvv4l2decoder ! queue ! mux.sink_0 \
         nvstreammux name=mux width=1920 height=1080 batch-size=1 ! queue ! nvvideoconvert ! \
         nvinfer config-file-path=/opt/nvidia/deepstream/deepstream-6.0/samples/configs/deepstream-app/config_infer_primary.txt \
         model-engine-file=/opt/nvidia/deepstream/deepstream-6.0/samples/models/Primary_Detector/resnet10.caffemodel_b1_gpu0_fp16.engine ! \
         queue ! nvdsosd process-mode=HW_MODE ! tee name=t \
         t. ! queue ! nvoverlaysink sync=false \
         t. ! queue ! nvvideoconvert ! video/x-raw,format=I420 ! x264enc tune=zerolatency bitrate=800 speed-preset=ultrafast ! \
         rtph264pay ! udpsink host=127.0.0.1 port=5000 sync=false async=false";

    pipeline = gst_parse_launch(pipeline_description, NULL);

    gst_element_set_state(pipeline, GST_STATE_PLAYING);
    g_main_loop_run(loop);

    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
    g_main_loop_unref(loop);

    return 0;
}
